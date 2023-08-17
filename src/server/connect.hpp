#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "logger.hpp"

class con_handler : public boost::enable_shared_from_this<con_handler>
{
    private:
        boost::asio::ip::tcp::socket sock;
        boost::asio::streambuf _buf;

    public:
        typedef boost::shared_ptr<con_handler> ptr;
        con_handler(boost::asio::io_service& io_service): sock(io_service) {}
        static ptr create(boost::asio::io_service& io_service);
        boost::asio::ip::tcp::socket& get_socket();
        void start();
        void handle_read(const boost::system::error_code& err, size_t byte_transferred);
        void handle_write(const boost::system::error_code& err, size_t byte_transferred);
        ~con_handler()
        {
            logger::get_info("DESTRUCTOR CON_HANDLER");
        }
};
