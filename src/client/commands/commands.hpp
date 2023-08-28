#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <vector>

class commands
{
private:
    boost::asio::streambuf _buf;
    std::array<uint8_t, sizeof(uint64_t)> _read_size;
    std::vector<uint8_t> _recv_msg;

    std::vector<uint8_t>  serialize_in_buf(std::string msg);
    std::string deserialize_from_buf();
public:
    void registration(boost::asio::ip::tcp::socket socket, std::string name, std::string sername, std::string patronymic);
    void commands::get_response(boost::asio::ip::tcp::socket socket);
    void get_info();
};
