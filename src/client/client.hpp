#pragma once

#include <boost/asio.hpp>
#include "user.hpp"
#include "command.hpp"

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
            void set_command(for_json::command& comm, const std::string& input);
            std::string get_response();
            std::vector<uint8_t> serialize_in_buf(std::string msg);

        private:
            boost::asio::io_service _io_service;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::streambuf _buf;
            uint64_t _read_size;
            std::vector<uint8_t> _recv_msg;
            for_json::user _user;
    };
}
