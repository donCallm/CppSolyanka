#include "connect.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

namespace net
{
    boost::asio::ip::tcp::socket& con_handler::get_socket()
    {
        return _sock;
    } 

    std::vector<std::string> con_handler::split_string(const std::string& input)
    { 
        std::vector<std::string> result;

        if (input.empty())
        {
            spdlog::info("EMPTY :(");
            return result;
        }

        std::istringstream iss(input);
        std::string token;

        while (std::getline(iss, token, ' ')) {
            result.push_back(token);
        }
        
        return result;
    }

    void con_handler::read_message(std::function<void(std::string)> callback)
    {
        boost::asio::async_read(_sock,
            boost::asio::buffer(_read_size),
            [self = shared_from_this(), callback](const boost::system::error_code& error, size_t bytes_transferred) {
                if (!error)
                {
                    if (self->_read_size.size() > 1024) callback("");

                    uint64_t received_value;
                    std::memcpy(&received_value, self->_read_size.data(), sizeof(uint64_t));
                    self->_recv_msg.resize(received_value);

                    boost::asio::async_read(self->_sock,
                        boost::asio::buffer(self->_recv_msg),
                        [self, callback](const boost::system::error_code& error, size_t bytes_transferred) {
                            if (!error)
                            {
                                std::string message(self->_recv_msg.begin(), self->_recv_msg.end());
                                callback(message);
                            }
                            else
                            {
                                std::cerr << "error: " << error.message() << std::endl;
                                self->_sock.close();
                                callback(""); 
                            }
                        });
                }
                else
                {
                    spdlog::error( "error: ", error.message());
                    self->_sock.close();
                    callback("");
                }
            });
    }

    void con_handler::start()
    {
        read_message([this](const std::string& message) {
            if (!message.empty()) 
            {
                spdlog::info("Received message from server: " + message);
                this->write_message("pong");
                this->start();
            } 
            else 
            {
                spdlog::info("Received empty message or error from server");
                this->write_message("Wrong message");
            }
        });
    }

    std::vector<uint8_t> con_handler::serialize(const std::string msg)
    {
        std::vector<uint8_t> serialized_msg;

        if (msg.empty())
        {
            spdlog::info("EMPTY :(");
            return serialized_msg;
        }
        
        uint64_t msg_size = msg.size();

        std::vector<uint8_t> size_bytes(sizeof(uint64_t));
        for (size_t i = 0; i < sizeof(uint64_t); ++i) 
            size_bytes[i] = (msg_size >> (i * 8)) & 0xFF;
        
        serialized_msg.insert(serialized_msg.end(), size_bytes.begin(), size_bytes.end());
        serialized_msg.insert(serialized_msg.end(), msg.begin(), msg.end());

        return serialized_msg;
    }

    void con_handler::write_message(std::string message)
    { 
        auto msg = serialize(message);

    boost::asio::async_write(_sock,
            boost::asio::buffer(msg.data(), msg.size()),
            boost::bind(&con_handler::handle_write,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void con_handler::handle_write(const boost::system::error_code& err, size_t byte_transferred)
    {
        if (!err)
        {
            std::cout << "Server sent something" << std::endl;
        }
        else
        {
            std::cerr << "error" << err.message() << std::endl;
            _sock.close();
        }
    }

    con_handler::ptr con_handler::create(boost::asio::io_service& io_service)
    {
        return ptr(new con_handler(io_service));
    }
}
