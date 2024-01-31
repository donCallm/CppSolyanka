#include "client.hpp"
#include <objects/message.hpp>
#include <objects/msg_objects.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

namespace core
{
    client::client(bool console_mode): _socket(_io_service), _id(0) { start(); }

    client::~client() { stop(); }

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

    void client::handler_result(const command::type& comm, const core::msg& rpl)
    {
        nlohmann::json json_data = nlohmann::json::parse(rpl.message);
        
        if (json_data.find("err_msg") != json_data.end())
        {
            error_msg err;
            err.from_json(json_data);
            spdlog::info("<< error: {}", err.message);
        }
        else if (json_data.find("res_msg") != json_data.end())
        {
            success_result_msg res;
            res.from_json(json_data);
            spdlog::info("<< response: {}", res.message);
            switch (comm)
            {
                case command::type::login: {
                    if (!rpl.params.empty())
                    {
                        _id = std::stoull(rpl.params[0]);
                    }
                    break; }
                default:
                    break;
            }
        }
        else
        {
            msg response;
            response.from_json(json_data);
            spdlog::info("<< response: {}", response.message);
        }
    }

    void client::executing()
    {
        core::message msg;
        core::command comm;
        core::msg rpl;
        spdlog::info("Enter command");

        while (true)
        {
            std::getline(std::cin, msg.data);

            if (msg.data.empty())
                continue;

            comm.set_command(msg.data);

            if (comm.instruction == command::type::end)
                stop();
            
            comm.params.push_back(std::to_string(_id));

            nlohmann::json serialize_message = comm; 
            std::string json_string = serialize_message.dump();
            write(json_string);
            
            rpl.from_json(nlohmann::json::parse(read_response()));
            handler_result(comm.instruction, rpl);

            comm.params.clear();
        }
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        _socket.connect(endpoint);
    }

    void client::start()
    {
        connect();
        read_hello_msg();
        std::async(std::launch::async, &client::executing, this);
    }

    void client::stop()
    {
        _socket.close();
    }
}
