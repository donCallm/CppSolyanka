#include "client.hpp"
#include "commands.hpp"
#include "message.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

namespace core
{
    void client::say_hello()
    {   
        _token = read_response();
    }

    std::string client::read_response()
    {
        try
        {
            std::size_t msg_size;
            boost::asio::read(_socket, boost::asio::buffer(&msg_size, sizeof(size_t)));
            
            if(_recv_msg.size() != msg_size)
                _recv_msg.resize(msg_size);

            boost::asio::read(_socket, boost::asio::buffer(_recv_msg.data(), msg_size));

            return std::string(_recv_msg.begin(), _recv_msg.end());
        }
        catch (const std::exception& e)
        {
            std::cerr << "error: " << e.what() << std::endl;
            _socket.close();
        }
        catch (...)
        {
            std::cerr << "underfind exception" << std::endl;
        }
    }

    void client::write(std::string data)
    {
        core::message msg;

        if (data.empty())
        {
            msg.data = "Error in server";
            spdlog::error("Message for send is empty");
        }
        else
        {
            msg.data = data;
        }

        _write_buff = core::serialize_message(msg);
        boost::asio::write(_socket, boost::asio::buffer(_write_buff.data(), _write_buff.size()));
    }

    void client::send_command()
    {
        core::message msg;
        objects::commands comm;

        while (true)
        {
            spdlog::info("Enter command");
            std::cin >> msg.data;

            comm.set_command(msg.data);
            comm.params.push_back(std::to_string(_user.id));
            comm.token = _token;

            nlohmann::json serialize_message = comm; 
            std::string json_string = serialize_message.dump();
            write(json_string);
            spdlog::info(read_response());

            spdlog::info(msg.data);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        _socket.connect(endpoint);
        say_hello();
        std::async(std::launch::async, &client::send_command, this);
    }
}
