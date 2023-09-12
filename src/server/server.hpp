#pragma once

#include "connect.hpp"

namespace core
{
    class server
    {
        private:
            boost::asio::ip::tcp::acceptor _acceptor;
            boost::asio::io_service& _io_service;

        private:
            void start_accept();    

        public:
            server(boost::asio::io_service& io_service);
        public:
            void handle_accept(net::con_handler::ptr connection, const boost::system::error_code& err);
            std::string get_client_status();
    };
}
