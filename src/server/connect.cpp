#include "connect.hpp"
#include "database.hpp"
#include <command.hpp>
#include "request_methods.hpp"
#include "connect.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

namespace net
{
    boost::asio::ip::tcp::socket& con_handler::get_socket()
    {
        return _sock;
    } 
    
    void con_handler::read_size()
    {
        if (_read_size.empty())
        {
            spdlog::info("Message without size");
        }
        if (_read_size.size() > 1024)
    
    void con_handler::read_size()
    {
        if (_read_size.empty())
        {
            spdlog::info("Message without size");
        }
        if (_read_size.size() > 1024)
        {
            spdlog::info("Message size exceeds allowable limit");
        }
        else
        {
            uint64_t received_value;
            std::memcpy(&received_value, _read_size.data(), sizeof(uint64_t));
            _recv_msg.resize(received_value);
        }
    }

    void con_handler::read_message(const boost::system::error_code& error)
    {
        if (!error)
        {
            for_json::command comm;
            std::string message = std::string(_recv_msg.begin(), _recv_msg.end());
            nlohmann::json json_string = nlohmann::json::parse(message);
            comm.from_json(json_string);

            spdlog::info("<<" + comm.comm);

            if (comm.comm == "ping") write_message("pong");
            else if (comm.comm == "registration") 
            accept_message();
        }
        else
        {   
            boost::asio::ip::tcp::endpoint endpoint = _sock.local_endpoint();
            spdlog::error("Clent " + endpoint.address().to_string() + ":" + std::to_string(endpoint.port()) + " disconnected");
            _sock.close();
        }
    }

    void con_handler::accept_message()
    {
        boost::asio::async_read(_sock,
            boost::asio::buffer(_read_size),
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
                if (!error)
                {
                    self->read_size();
                    self->read_size();

                    boost::asio::async_read(self->_sock,
                        boost::asio::buffer(self->_recv_msg),
                        [self](const boost::system::error_code& error, size_t bytes_transferred) { self->read_message(error); });
                        [self](const boost::system::error_code& error, size_t bytes_transferred) { self->read_message(error); });
                }
                else
                {
                    spdlog::error("Clent disconnected");
                    spdlog::error("Clent disconnected");
                    self->_sock.close();
                }
            });
    }

    void con_handler::start()
    {
        accept_message();
        accept_message();
    }

    std::vector<uint8_t> con_handler::serialize(const std::string msg)
    {
        std::vector<uint8_t> serialized_msg;
        serialized_msg.push_back(msg.size());
        for (int i = 0; i < 7; ++i) serialized_msg.push_back(0);
        serialized_msg.push_back(msg.size());
        for (int i = 0; i < 7; ++i) serialized_msg.push_back(0);
        serialized_msg.insert(serialized_msg.end(), msg.begin(), msg.end());

        return serialized_msg;
    }

    void con_handler::write_message(std::string message)
    { 
        std::vector<uint8_t> msg = serialize(message);
        std::vector<uint8_t> msg = serialize(message);

        boost::asio::async_write(_sock,
        boost::asio::async_write(_sock,
            boost::asio::buffer(msg.data(), msg.size()),
            boost::bind(&con_handler::handle_write,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void con_handler::handle_write(const boost::system::error_code& err, size_t byte_transferred)
    {
        if (err)
        if (err)
        {
            spdlog::error("Write error: ", err.message());
            spdlog::error("Write error: ", err.message());
            _sock.close();
        }
    }

    net::con_handler::ptr con_handler::create(boost::asio::io_service& io_service) 
    net::con_handler::ptr con_handler::create(boost::asio::io_service& io_service) 
    {
        return std::make_shared<con_handler>(io_service);
    }

    con_handler::~con_handler() 
    {
        spdlog::error("Clent disconnected"); 
        return std::make_shared<con_handler>(io_service);
    }

    con_handler::~con_handler() 
    {
        spdlog::error("Clent disconnected"); 
    }
}
