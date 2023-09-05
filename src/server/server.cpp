#include "server.hpp"

void server::start_accept()
{
    logger::get_info("Wait new user\n");
    con_handler::ptr connection = con_handler::create(_io_service);
    
    _acceptor.async_accept(connection->get_socket(), 
        boost::bind(&server::handle_accept, this,  connection, boost::placeholders::_1));
}

server::server(boost::asio::io_service& io_service): _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080)),
_io_service(io_service), _connection_count(0)
{
    logger::get_info(utlis::toc);
    start_accept();
}

void server::handle_accept(con_handler::ptr connection, const boost::system::error_code& err)
{
    logger::get_info("HANDLE ACCEPT");
    if (!err)
    {
        connection->start();
    }

    start_accept();
}
