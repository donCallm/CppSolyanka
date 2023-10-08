#include "client.hpp"
#include "commands.hpp"
#include "message.hpp"
#include "reply.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

namespace core
{
    void client::read_hello_msg()
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
            throw std::runtime_error("data for write is empty");
        }
        else
        {
            msg.data = data;
        }

        _write_buff = core::serialize_message(msg);
        boost::asio::write(_socket, boost::asio::buffer(_write_buff.data(), _write_buff.size()));
    }

    void client::start()
    {
        core::message msg;
        core::commands comm;
        core::reply rpl;

        while (true)
        {
            spdlog::info("Enter command");
            std::getline(std::cin, msg.data);

            comm.set_command(msg.data);
            comm.params.push_back(std::to_string(_user_id));
            comm.token = _token;

            nlohmann::json serialize_message = comm; 
            std::string json_string = serialize_message.dump();
            write(json_string);

            rpl.from_json(nlohmann::json::parse(read_response()));
            spdlog::info("<< response: {}", rpl.msg);
            if (comm.instruction == "registration") 
                if (!rpl.params.empty()) _user_id = std::stoull(rpl.params[0]);
        }
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        _socket.connect(endpoint);
        read_hello_msg();
        std::async(std::launch::async, &client::start, this);
    }
}
