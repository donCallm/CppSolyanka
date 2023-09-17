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
            std::string message = std::string(_recv_msg.begin(), _recv_msg.end());
            spdlog::info("<<" + message);

            if (message == "ping") write_message("pong");
            accept_message();
        }
        else
        {
            spdlog::error("Read error: ", error.message());
            _sock.close();
        }
    }

    void con_handler::accept_message()
    {
        boost::asio::async_read(_sock,
            boost::asio::buffer(_read_size),
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
                if (!error)
                {
                    self->read_size();

                    boost::asio::async_read(self->_sock,
                        boost::asio::buffer(self->_recv_msg),
                        [self](const boost::system::error_code& error, size_t bytes_transferred) { self->read_message(error); });
                }
                else
                {
                    if (error == boost::asio::error::eof) spdlog::error("Clent disconnected");
                    else spdlog::error("Read error: ", error.message());
                    self->_sock.close();
                }
            });
    }

    void con_handler::start()
    {
        accept_message();
    }

    std::vector<uint8_t> con_handler::serialize(const std::string msg)
    {
        std::vector<uint8_t> serialized_msg;
        serialized_msg.push_back(msg.size());
        for (int i = 0; i < 7; ++i) serialized_msg.push_back(0);
        serialized_msg.insert(serialized_msg.end(), msg.begin(), msg.end());

        return serialized_msg;
    }

    void con_handler::write_message(std::string message)
    { 
        std::vector<uint8_t> msg = serialize(message);

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
        {
            spdlog::error("Write error: ", err.message());
            _sock.close();
        }
    }

    net::con_handler::ptr con_handler::create(boost::asio::io_service& io_service) 
    {
        return std::make_shared<con_handler>(io_service);
    }

    con_handler::~con_handler() 
    {
         spdlog::info("Con_handler destructor"); 
    }
}
