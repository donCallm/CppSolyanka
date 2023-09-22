#include "client.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace core
{
    void client::set_command(for_json::command& comm, const std::string& input)
    { 
        std::istringstream iss(input);
        std::string token;
        std::getline(iss, token, ' ');
        comm.comm = token;
        while (std::getline(iss, token, ' ')) { comm.params.push_back(token); }
        comm.params.push_back(std::to_string(_user.id));
    }

    std::vector<uint8_t> client::serialize_in_buf(std::string msg)
    {
        std::vector<uint8_t> serialized_msg;
        serialized_msg.push_back(msg.size());
        for (int i = 0; i < 7; ++i) serialized_msg.push_back(0);
        serialized_msg.insert(serialized_msg.end(), msg.begin(), msg.end());

        return serialized_msg;
    }

    std::string client::get_response()
    {
        try
        {
            uint64_t received_value;
            boost::asio::read(_socket, boost::asio::buffer(&received_value, sizeof(uint64_t)));

            _recv_msg.resize(received_value);
            boost::asio::read(_socket, boost::asio::buffer(_recv_msg));

            spdlog::info("<<" + std::string(_recv_msg.begin(), _recv_msg.end()));
            return std::string(_recv_msg.begin(), _recv_msg.end());
        }
        catch (const boost::system::system_error& e)
        {
            std::cerr << "error: " << e.what() << std::endl;
            _socket.close();
        }
        return "";
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
            //boost::system::error_code err;
            for_json::command comm;
            set_command(comm, "ping");
            nlohmann::json serialize_message = comm;
            std::string json_string = serialize_message.dump();
            write(json_string);
            get_response();
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    }

    void client::send_command()
    {
        for_json::command comm;
        std::string message;

        spdlog::info("Enter command:");
        std::cin >> message;
        set_command(comm, message);

        nlohmann::json serialize_message = comm;
        std::string json_string = serialize_message.dump();

        write(json_string);
        nlohmann::json response = nlohmann::json::parse(get_response());

        if (comm.comm == "registration")
        {
            _user.from_json(response);
        }

        std::async(std::launch::async, &client::send_command, this);
    }

    void client::connect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        _socket.connect(endpoint);
        std::thread thread1(&client::send_command, this);
        std::thread thread2(&client::ping, this);

        thread1.join();
        thread2.join();
    }
}
