#include "client.hpp"
#include "commands.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

namespace core
{
    void client::set_command(objects::commands& comm, const std::string& input)
    { 
        std::vector<std::string> result;
        std::istringstream iss(input);
        std::string token;

        std::getline(iss, token, ' ');
        comm.comm = token;
        while (std::getline(iss, token, ' ')) { comm.params.push_back(token); }
        comm.params.push_back(std::to_string(_user.id));
    }

    std::vector<uint8_t> client::serialize_in_buf(std::string msg)
    {
        uint8_t msg_size = msg.size();

        std::vector<uint8_t> serialize_msg;
        serialize_msg.insert(serialize_msg.end(), msg_size);
        serialize_msg.insert(serialize_msg.end(), msg.begin(), msg.end());

        return serialize_msg;
    }

    void client::get_response()
    {
        try
        {
            uint8_t received_value;
            boost::asio::read(_socket, boost::asio::buffer(&received_value, sizeof(uint8_t)));

            _recv_msg.resize(received_value);
            boost::asio::read(_socket, boost::asio::buffer(_recv_msg));

            spdlog::info("<<" + std::string(_recv_msg.begin(), _recv_msg.end()));
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

    void client::write(std::string msg)
    {
        boost::system::error_code err;
        std::vector<uint8_t> serialize_msg = serialize_in_buf(msg);
        boost::asio::write(_socket, boost::asio::buffer(serialize_msg), err);
    }

    void client::send_command()
    {
        std::string message;
        objects::commands comm;
        while (true)
        {
            spdlog::info("Enter command");
            std::cin >> message;
            set_command(comm, message);
            nlohmann::json serialize_message = comm; 
            std::string json_string = serialize_message.dump();
            write(json_string);
            get_response();
        }
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        _socket.connect(endpoint);
        std::async(std::launch::async, &client::send_command, this);
    }
}
