#include "server.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

namespace core
{
    void server::start_accept()
    {
        spdlog::info("Wait new user");
        net::con_handler::ptr connection = net::con_handler::create(_io_service);
        
        _acceptor.async_accept(connection->get_socket(), 
            boost::bind(&server::handle_accept, this,  connection, boost::placeholders::_1));
    }

    server::server(boost::asio::io_service& io_service): _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080)),
    _io_service(io_service)
    {   
        boost::asio::ip::tcp::endpoint endpoint = _acceptor.local_endpoint();
        spdlog::info("New client - " +  endpoint.address().to_string() + ":" + std::to_string(endpoint.port()));
        start_accept();
    }

    void server::handle_accept(net::con_handler::ptr connection, const boost::system::error_code& err)
    {
        if (!err)
        {
            connection->start();
        } 
        else
        {
            spdlog::error("Connecting error: " + err.message());
        } 

        start_accept();
    }
}
