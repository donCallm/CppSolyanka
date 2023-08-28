#include "client.hpp"
#include <thread>
#include <chrono>

void client::connect()
{
    auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
    _socket.connect(endpoint);
    std::cout << "Connect to: " << endpoint << std::endl;
    sent_request();
}

