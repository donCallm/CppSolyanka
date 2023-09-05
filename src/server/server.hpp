#pragma once

#include "connect.hpp"
#include "utlis.hpp"

class server
{
    private:
        boost::asio::ip::tcp::acceptor _acceptor;
        boost::asio::io_service& _io_service;
        int _connection_count;

    void start_accept();

    public:
        server(boost::asio::io_service& io_service);
        void handle_accept(con_handler::ptr connection, const boost::system::error_code& err);
        std::string get_client_status();
        ~server()
        {
            logger::get_info("DESTRUCTOR SERVER");
        }
};
