#include "client.hpp"

void client::connect()
{
    _socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080));
}

void client::sent_request()
{
    std::string msg = "EGOR PIDOR\n";
    boost::system::error_code err;
    boost::asio::write(_socket, boost::asio::buffer(msg), err);
    get_response();
}

void client::get_response()
{
    boost::asio::streambuf buf;
    boost::system::error_code err;
    boost::asio::read(_socket, buf, boost::asio::transfer_all(), err);
    if(err && err != boost::asio::error::eof ) 
    {
        std::cout << "receive failed: " << err.message() << std::endl;
    }
    else 
    {
        const char* data = boost::asio::buffer_cast<const char*>(buf.data());
        std::cout << data << std::endl;
    }
}