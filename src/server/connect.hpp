#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

namespace net
{
    class con_handler : public std::enable_shared_from_this<con_handler>
    {
        public:
            typedef std::shared_ptr<con_handler> ptr;
            explicit con_handler(boost::asio::io_service& io_service): _sock(io_service) {}
            ~con_handler();

        private:
            void write_message(std::string message);
            void handle_write(const boost::system::error_code& err, size_t byte_transferred);
            void read_size();
            void accept_message();
            void read_message(const boost::system::error_code& error);
            std::vector<uint8_t> serialize(const std::string msg);

        public:
            void start();
            static ptr create(boost::asio::io_service& io_service);
            boost::asio::ip::tcp::socket& get_socket();

        private:
            boost::asio::ip::tcp::socket _sock;
            ptr _pointer;
            boost::asio::streambuf _buf;
            std::array<uint8_t, sizeof(uint8_t)> _read_size;
            std::vector<uint8_t> _recv_msg;
            const uint8_t max_size = 255;
    };
}
