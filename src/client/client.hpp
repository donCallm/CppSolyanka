#pragma once

#include <boost/asio.hpp>

namespace core
{
    class client
    {
        private:
            boost::asio::io_service _io_service;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::streambuf _buf;
            uint64_t _read_size;
            std::vector<uint8_t> _recv_msg;

        public:
            client(): _socket(_io_service){connect();}

        public:
            void get_response();
            void ping();
            void write(std::string msg);
            void connect();
            std::vector<uint8_t> serialize_in_buf(std::string msg);
    };
}
