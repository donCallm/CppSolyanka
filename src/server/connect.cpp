#include "connect.hpp"

boost::asio::ip::tcp::socket& con_handler::get_socket()
{
    return sock;
} 

void con_handler::start()
{
    auto ptr = shared_from_this();
    
    boost::asio::async_read(sock,
        boost::asio::buffer(_data, max_length),
        boost::bind(&con_handler::handle_read,
        ptr,
        boost::placeholders::_1,
        boost::placeholders::_2));

    boost::asio::async_write(sock,
        boost::asio::buffer("DAROVA ZAEBAL\n", max_length),
        boost::bind(&con_handler::handle_write,
        ptr,
        boost::placeholders::_1,
        boost::placeholders::_2));
}

con_handler::ptr con_handler::create(boost::asio::io_service& io_service)
{
    return ptr(new con_handler(io_service));
}

void con_handler::handle_read(const boost::system::error_code& err, size_t byte_transferred)
{
    if (!err)
    {
        std::cout << _data << std::endl;
    }
    else
    {
        std::cerr << "error" << err.message() << std::endl;
        sock.close();
    }
}
void con_handler::handle_write(const boost::system::error_code& err, size_t byte_transferred)
{
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
