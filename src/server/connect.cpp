#include "connect.hpp"

boost::asio::ip::tcp::socket& con_handler::get_socket()
{
    return sock;
} 

void con_handler::start()
{
    logger::get_info("TEST START");

    boost::system::error_code err;

    boost::asio::async_read(sock,
        _buf,
        boost::asio::transfer_all(),
        boost::bind(&con_handler::handle_read,
        shared_from_this(),
        err,
        boost::placeholders::_2));
    logger::get_info("AFTER READ");
    boost::asio::async_write(sock,
        boost::asio::buffer("DAROVA ZAEBAL\n"),
        boost::bind(&con_handler::handle_write,
        shared_from_this(),
        err,
        boost::asio::placeholders::bytes_transferred));
        logger::get_info("AFTER ASYNC WRITE");
}

con_handler::ptr con_handler::create(boost::asio::io_service& io_service)
{
    return ptr(new con_handler(io_service));
}

void con_handler::handle_read(const boost::system::error_code& err, size_t byte_transferred)
{
    logger::get_info("TEST HANDLE READ");

    if (!err)
    {
        const char* data = boost::asio::buffer_cast<const char*>(_buf.data());
        std::cout << "Server Response: " << data << std::endl;
    }
    else
    {
        std::cerr << "error" << err.message() << std::endl;
        sock.close();
    }
}
void con_handler::handle_write(const boost::system::error_code& err, size_t byte_transferred)
{
    logger::get_info("TEST HANDLE WRITE");
    if (!err)
    {
        std::cout << "Server sent something" << std::endl;
    }
    else
    {
        std::cerr << "error" << err.message() << std::endl;
        sock.close();
    }
}
