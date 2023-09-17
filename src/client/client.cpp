#include "client.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>

namespace core
{
    std::vector<std::string> split_string(const std::string& input)
    { 
        std::vector<std::string> result;
        std::istringstream iss(input);
        std::string token;

        while (std::getline(iss, token, ' ')) { result.push_back(token); }

        return result;
    }

    std::vector<uint8_t> client::serialize_in_buf(std::string msg)
    {
        uint64_t msg_size = msg.size();
        std::vector<uint8_t> size_bites(sizeof(uint64_t));
        for (size_t i = 0; i < sizeof(uint64_t); ++i) 
            size_bites[i] = (msg_size >> (i * 8)) & 0xFF;

        std::vector<uint8_t> serialize_msg;
        serialize_msg.insert(serialize_msg.end(), size_bites.begin(), size_bites.end());
        serialize_msg.insert(serialize_msg.end(), msg.begin(), msg.end());

        return serialize_msg;
    }

    void client::get_response()
    {
        try
        {
            uint64_t received_value;
            boost::asio::read(_socket, boost::asio::buffer(_read_size));
            std::memcpy(&received_value, _read_size.data(), sizeof(uint64_t));

            _recv_msg.resize(received_value);
            boost::asio::read(_socket, boost::asio::buffer(_recv_msg));

            spdlog::info("Received message: " + std::string(_recv_msg.begin(), _recv_msg.end()));
        }
        catch (const boost::system::system_error& e)
        {
            std::cerr << "error: " << e.what() << std::endl;
            _socket.close();
        }
    }

    void client::write(std::string msg)
    {
        boost::system::error_code err;
        std::vector<uint8_t> serialize_msg = serialize_in_buf(msg);
        boost::asio::write(_socket, boost::asio::buffer(serialize_msg), err);
    }

    void client::ping()
    {
        while (true)
        {
            boost::system::error_code err;
            write("ping");
            get_response();
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        _socket.connect(endpoint);
        std::async(std::launch::async, &client::ping, this);
    }
}
