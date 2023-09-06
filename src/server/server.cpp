#include "server.hpp"

namespace core
{
    void server::start_accept()
    {
        spdlog::info("Wait new user\n");
        net::con_handler::ptr connection = net::con_handler::create(_io_service);
        
        _acceptor.async_accept(connection->get_socket(), 
            boost::bind(&server::handle_accept, this,  connection, boost::placeholders::_1));
    }

    server::server(boost::asio::io_service& io_service): _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080)),
    _io_service(io_service)
    {
        start_accept();
    }

    void server::handle_accept(net::con_handler::ptr connection, const boost::system::error_code& err)
    {
        spdlog::info("HANDLE ACCEPT");
        if (!err)
        {
            connection->start();
        }

        start_accept();
    }
}
