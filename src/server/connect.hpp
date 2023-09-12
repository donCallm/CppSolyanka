#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace net
{
    class con_handler : public boost::enable_shared_from_this<con_handler>
    {
        private:
            boost::asio::ip::tcp::socket _sock;
            boost::asio::streambuf _buf;
            std::array<uint8_t, sizeof(uint64_t)> _read_size;
            std::vector<uint8_t> _recv_msg;

        private:
            void read_message(std::function<void(std::string)> callback);
            void write_message(std::string message);
            void handle_write(const boost::system::error_code& err, size_t byte_transferred);
            void handle_read();
            std::vector<uint8_t> serialize(const std::string msg);
            std::vector<std::string> split_string(const std::string& input);

        public:
            typedef boost::shared_ptr<con_handler> ptr;
            explicit con_handler(boost::asio::io_service& io_service): _sock(io_service) {}

        public:
            void start();
            static ptr create(boost::asio::io_service& io_service);
            boost::asio::ip::tcp::socket& get_socket();
    };
}
