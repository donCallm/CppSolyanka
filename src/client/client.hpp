#pragma once
#include <boost/asio.hpp>
#include <objects/commands.hpp>

namespace core
{
    class msg;
    class command;

    class client
    {
        public:
            client(int con_port);
            ~client();

        public:
            void write(std::string& msg);
            void connect();
            void handler_result(const command::type& comm, const core::msg& rpl);
            std::string read_response();
            void stop();
            void start();
            void executing();

        private:
            boost::asio::io_service _io_service;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::streambuf _buf;
            std::vector<uint8_t> _recv_msg;
            std::vector<uint8_t> _write_buff;
            uint64_t _id;
            int _port;
    };
}
