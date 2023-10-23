#include "connect.hpp"
#include "commands.hpp"
#include "message.hpp"
#include "reply.hpp"
#include "success_result.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

namespace net
{
    con_handler::con_handler(boost::asio::io_service& io_service): _sock(io_service) {}
    
    boost::asio::ip::tcp::socket& con_handler::get_socket()
    {
        return _sock;
    } 

    void con_handler::lack_of_token()
    {
        boost::asio::ip::tcp::endpoint remote_endpoint = _sock.remote_endpoint();
        spdlog::error("client {} send message without token", remote_endpoint.address().to_string() + ":" + std::to_string(remote_endpoint.port()));
        write_message("pls restart app");
        _sock.close();
    }

    void con_handler::invok_func(core::commands& comm)
    {
        core::success_result res;
        core::reply rpl;
        switch (comm.instruction)
        {
            case core::commands::registration: {
                rpl.msg = _state.registration(comm, _client);
                break; }
            case core::commands::login: {
                rpl.msg = _state.login(comm, _client);
                break; }
            case core::commands::ping: {
                res.result_msg = "ping";
                rpl.msg = res.get_json();
                spdlog::info("server sending pong");
                break; }
            case core::commands::end: {
                spdlog::info("client {} wants to disconnect", get_adress());
                _sock.close();
                break; }
            default: {
                res.result_msg = "unknown_command";
                rpl.msg = res.get_json();
                spdlog::warn("server sending message about uncknow command");
                break; }
        }
        nlohmann::json serialize_message = rpl; 
        std::string json_string = serialize_message.dump();
        write_message(json_string);
    }

    std::string con_handler::get_adress()
    {
        boost::asio::ip::tcp::endpoint remote_endpoint = _sock.remote_endpoint();
        return remote_endpoint.address().to_string() + ":" + std::to_string(remote_endpoint.port());
    }

    void con_handler::on_msg_ready()
    {
        core::commands comm;
        core::message msg = core::deserialize_message(_read_buff, _msg_size);
        comm.from_json(nlohmann::json::parse(msg.data));
        
        spdlog::info("<< receive from client {}: command - {}, command id - {}", get_adress(), comm.instruction, comm.id);

        if (comm.token != _token)
        {
            lack_of_token();
        }
        else
        {
            invok_func(comm);
            accept_message();
        }
    }

    void con_handler::say_hello()
    {
        spdlog::info(">> client {} connected", get_adress());
        spdlog::info(">> server say hello");
        write_message(_token);
    }

    void con_handler::read_message()
    {
        if (_read_buff.size() < _msg_size)
            _read_buff.resize(_msg_size);

        auto self = shared_from_this();
        boost::asio::async_read(_sock, boost::asio::buffer(_read_buff.data(), _msg_size),
            [self](const boost::system::error_code& e, std::size_t transferred)
            {
                if (e)
                    spdlog::error("error: {}", e.message());

                assert(transferred == self->_msg_size);
                self->on_msg_ready();
            });
    }

    void con_handler::accept_message()
    {
        boost::asio::async_read(_sock,
            boost::asio::buffer(&_msg_size, sizeof(size_t)),
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
                if (!error)
                {
                    self->read_message();
                }
                else
                {
                    self->_sock.close();
                }
            });
    }

    void con_handler::start()
    {
        _state.setup();
        say_hello();
        accept_message();
    }

    void con_handler::write_message(const std::string& data)
    { 
        core::message msg;
        msg.data = data;
        _write_buff = core::serialize_message(msg);

        boost::asio::async_write(_sock,
            boost::asio::buffer(_write_buff.data(), _write_buff.size()),
            boost::bind(&con_handler::handle_write,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void con_handler::handle_write(const boost::system::error_code& err, size_t byte_transferred)
    {
        if (err)
        {
            spdlog::error("write error: {}", err.message());
            _sock.close();
        }
    }

    net::con_handler::ptr con_handler::create(boost::asio::io_service& io_service) 
    {
        return std::make_shared<con_handler>(io_service);
    }

    con_handler::~con_handler() 
    {
        spdlog::info("client disconected");
    }
}
