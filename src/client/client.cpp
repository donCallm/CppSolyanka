#include "client.hpp"
#include <thread>
#include <chrono>

void client::connect()
{
    auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
    _socket.connect(endpoint);
    std::cout << "Connect to: " << endpoint << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    sent_request();
}

void client::sent_request()
{
    std::cout << "TEST SENT REQUEST/n" << std::endl;
    std::string msg = "EGOR PIDOR\n";
    boost::system::error_code err;
    boost::asio::write(_socket, boost::asio::buffer(msg), err);
    get_response();
    _socket.close();
}

void client::get_response()
{
    std::cout << "TEST GET REPONSE\n" << std::endl;
    boost::asio::streambuf buf;
    boost::system::error_code err;
    boost::asio::read(_socket, buf, boost::asio::transfer_all(), err);
    if(err) 
    {
        std::cout << "ERROR: " << err.message() << std::endl;
        _socket.close();
    }
    else 
    {
        const char* data = boost::asio::buffer_cast<const char*>(buf.data());
        std::cout << "Server Response: " << data << std::endl;
    }
}
