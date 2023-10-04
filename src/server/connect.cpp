#include "connect.hpp"
#include "commands.hpp"
#include "message.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

namespace net
{
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
        if (comm.instruction == "ping")
        {
            write_message("pong");
            spdlog::info("server sending pong");
        }
        else
        {
            write_message("uncknow command");
            spdlog::info("server sending message about uncknow command");
        }
    }

    void con_handler::on_msg_ready()
    {
        core::commands comm;
        core::message msg = core::deserialize_message(_read_buff, _msg_size);
        comm.from_json(nlohmann::json::parse(msg.data));
        
        spdlog::info("<< " + comm.instruction);

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
        boost::asio::ip::tcp::endpoint remote_endpoint = _sock.remote_endpoint();
        spdlog::info("client {} connected", remote_endpoint.address().to_string() + ":" + std::to_string(remote_endpoint.port()));
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
