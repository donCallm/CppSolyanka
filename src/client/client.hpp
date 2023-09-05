#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

class client
{
private:
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::streambuf _buf;
    std::array<uint8_t, sizeof(uint64_t)> _read_size;
    std::vector<uint8_t> _recv_msg;

public:
    void get_response();
    void ping();
    void write(std::string msg);
    std::vector<uint8_t> serialize_in_buf(std::string msg);
    std::vector<std::string> split_string(const std::string& input);
    client(): _socket(_io_service){connect();}
    void connect();
};
