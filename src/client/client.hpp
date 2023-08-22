#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <vector>

class client
{
private:
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::streambuf _buf;
    std::array<uint8_t, sizeof(uint64_t)> _read_size;
    std::vector<uint8_t> _recv_msg;

public:
    client(): _socket(_io_service){connect();}
    std::vector<uint8_t>  serialize_in_buf(std::string msg);
    std::string deserialize_from_buf();
    void connect();
    void sent_request();
    void get_response();
    ~client()
    {
        std::cout << "I DIE" << std::endl;
    }
};
