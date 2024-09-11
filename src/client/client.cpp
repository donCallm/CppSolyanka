#include "client.hpp"
#include <objects/message.hpp>
#include <objects/msg_objects.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

namespace core
{
    client::client(int con_port): _socket(_io_service), _id(0), _port(con_port) { start(); }

    client::~client() { stop(); }

    std::string client::read_response()
    {
        std::size_t msg_size;
        boost::asio::read(_socket, boost::asio::buffer(&msg_size, sizeof(size_t)));
        
        if(_recv_msg.size() != msg_size)
            _recv_msg.resize(msg_size);

        boost::asio::read(_socket, boost::asio::buffer(_recv_msg.data(), msg_size));
        return std::string(_recv_msg.begin(), _recv_msg.end());
    }

    void client::write(std::string& data)
    {
        core::message msg;

        if (data.empty())
            throw std::runtime_error("data for write is empty");
        else
            msg.data = data;

        _write_buff = core::serialize_message(msg);
        boost::asio::write(_socket, boost::asio::buffer(_write_buff.data(), _write_buff.size()));
    }

    void client::handler_result(const command::type& comm, const core::msg& rpl)
    {
        spdlog::info("Response: {}", rpl.message);
    }

    void client::executing()
    {
        core::message msg;
        core::msg rpl;
        core::command comm;
        spdlog::info("Enter command");

        while (true)
        {
            std::getline(std::cin, msg.data);

            comm.set_command(msg.data);
            comm.params.push_back(std::to_string(_id));
            switch (comm.instruction)
            {
                case command::type::end:
                {
                    std::string tmp("end");
                    write(tmp);
                    stop();
                    return;
                }
                case command::type::ping:
                {
                    std::string tmp("ping");
                    write(tmp);
                    break;
                }
                default:
                {
                    std::string tmp("unknown command");
                    write(tmp);
                    break;
                }
            }
            
            try
            {
                rpl.set_message(read_response());
            }
            catch (const std::exception& e)
            {
                spdlog::error("error: {}", e.what());
                stop();
                break;
            }

            handler_result(comm.instruction, rpl);

            comm.params.clear();
        }
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), _port);
        _socket.connect(endpoint);
    }

    void client::start()
    {
        connect();
        std::async(std::launch::async, &client::executing, this);
    }

    void client::stop()
    {
        if (_io_service.stopped()) 
            return;

        if (_socket.is_open()) 
            _socket.close();

        _io_service.stop();
        spdlog::info("Shutdown client");
    }
}
