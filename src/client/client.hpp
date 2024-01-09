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
            client(bool console_mode);
            client();
            virtual ~client() = default;

        public:
            virtual void read_hello_msg();
            virtual void write(std::string msg);
            virtual void connect();
            virtual void start();
            virtual void executing();
            virtual void handler_result(const command::type& comm, const core::msg& rpl);
            virtual std::string read_response();


        private:
            boost::asio::io_service _io_service;
            boost::asio::ip::tcp::socket _socket;
            boost::asio::streambuf _buf;
            uint8_t _read_size;
            std::vector<uint8_t> _recv_msg;
            std::vector<uint8_t> _write_buff;
            std::string _token;
            bool _console_mode;
            uint64_t _id;
    };
}
