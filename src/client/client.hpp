#pragma once

#include <boost/asio.hpp>
#include <objects/user.hpp>

namespace core
{
    class client
    {
        public:
            client(bool console_mode = true);

        public:
            void ping();
            void read_hello_msg();
            void write(std::string msg);
            void connect();
            void start();
            std::string read_response();
            std::vector<uint8_t> serialize_in_buf(std::string msg);

        private:
            boost::asio::io_service _io_service;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::streambuf _buf;
            uint8_t _read_size;
            std::vector<uint8_t> _recv_msg;
            std::vector<uint8_t> _write_buff;
            std::string _token;
            bool _console_mode;
    };
}
