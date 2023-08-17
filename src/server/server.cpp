#include "server.hpp"

void server::start_accept()
{
    con_handler::ptr connection = con_handler::create(io_service);
    
    _acceptor.async_accept(connection->get_socket(), 
        boost::bind(&server::handle_accept, this, connection, boost::placeholders::_1));

    boost::asio::ip::tcp::endpoint temp = _acceptor.local_endpoint();
    logger::get_info("New Client: ");
    logger::get_info("IP: " + temp.address().to_string());
    logger::get_info("PORT: " + std::to_string(temp.port()));
}

server::server(boost::asio::io_service& io_service): _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080))
{
    logger::get_info(utlis::toc);
    start_accept();
}

void server::handle_accept(con_handler::ptr connection, const boost::system::error_code& err)
{
    if (!err)
    {
        logger::get_info("TEST HANDLE ACCEPT");
        connection->start();
        logger::get_info("AFTER START");
    }
    start_accept();
    logger::get_info("AFTER NEW ACCEPT");
}
