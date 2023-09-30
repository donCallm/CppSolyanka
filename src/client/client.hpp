#pragma once

#include <boost/asio.hpp>
#include "commands.hpp"
#include "user.hpp"

namespace core
{
    class client
    {
        public:
            client(): _socket(_io_service){connect();}

        public:
            void ping();
            void write(std::string msg);
            void connect();
            void send_command();
            void read_response();
            void set_command(objects::commands& comm, const std::string& input);
            std::vector<uint8_t> serialize_in_buf(std::string msg);

        private:
            boost::asio::io_service _io_service;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::streambuf _buf;
            uint8_t _read_size;
            std::vector<uint8_t> _recv_msg;
            std::vector<uint8_t> _write_buff;
            objects::user _user;
    };
}
