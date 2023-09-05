#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <memory>
#include <thread>
#include <chrono>
#include "logger.hpp"

class con_handler : public boost::enable_shared_from_this<con_handler>
{
    private:
        boost::asio::ip::tcp::socket sock;
        boost::asio::streambuf _buf;
        std::array<uint8_t, sizeof(uint64_t)> _read_size;
        std::vector<uint8_t> _recv_msg;
        bool _started; 
        bool _read_in_progress;
        void read_message(std::function<void(std::string)> callback);
        void write_message(std::string message);
        void handle_write(const boost::system::error_code& err, size_t byte_transferred);
        std::vector<uint8_t> serialize(const std::string msg);
        std::vector<std::string> split_string(const std::string& input);
        void handle_read();

    public:
        void start();
        bool is_started() const;
        void set_started(bool value);
        typedef boost::shared_ptr<con_handler> ptr;
        explicit con_handler(boost::asio::io_service& io_service): sock(io_service), _read_in_progress(false) {}
        static ptr create(boost::asio::io_service& io_service);
        boost::asio::ip::tcp::socket& get_socket();
        ~con_handler()
        {
            logger::get_info("DESTRUCTOR CON_HANDLER");
        }
};
