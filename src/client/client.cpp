#include "client.hpp"
#include <objects/commands.hpp>
#include <objects/message.hpp>
#include <objects/msg_objects.hpp>
#include <iostream>
#include <spdlog/spdlog.h>
using namespace core;

namespace core
{
    client::client(bool console_mode): _socket(_io_service) {connect();}

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
            throw std::runtime_error("data for write is empty");
        else
            msg.data = data;

        _write_buff = core::serialize_message(msg);
        boost::asio::write(_socket, boost::asio::buffer(_write_buff.data(), _write_buff.size()));
    }

    void client::handler_result(const reply_msg& rpl)
    {
        nlohmann::json json_data = nlohmann::json::parse(rpl.reply_msg);

        if (json_data.find("error_msg") != json_data.end())
        {
            error_msg err;
            err.from_json(json_data);
            spdlog::info("error: {}", err.error_msg);
        }
        else
        {
            success_result_msg res;
            res.from_json(json_data);
            spdlog::info("<< response: {}", res.result_msg);
        }
    }

    void client::start()
    {
        message msg;
        command comm;
        reply_msg rpl;

        while (true)
        {
            spdlog::info("Enter command");
            std::getline(std::cin, msg.data);

            comm.set_command(msg.data);
            comm.token = _token;

            nlohmann::json serialize_message = comm; 
            std::string json_string = serialize_message.dump();
            write(json_string);

            rpl.from_json(nlohmann::json::parse(read_response()));
            handler_result(rpl);

            comm.params.clear();
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
