#include "client.hpp"
#include <thread>
#include <chrono>

void client::connect()
{   
    std::cout << "\n" << std::endl;
    std::cout << "CONNECT" << std::endl;
    auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
    _socket.connect(endpoint);
    std::cout << "Connect to: " << endpoint << std::endl;
    sent_request();
    std::cout << "\n" << std::endl;
}

std::vector<uint8_t> client::serialize_in_buf(std::string msg)
{
    std::cout << "\n" << std::endl;
    std::cout << "SERIALIZE" << std::endl;

    uint64_t msg_size = msg.size();
    std::vector<uint8_t> size_bites(sizeof(uint64_t));
    for (size_t i = 0; i < sizeof(uint64_t); ++i) 
        size_bites[i] = (msg_size >> (i * 8)) & 0xFF;

    std::vector<uint8_t> serialize_msg;
    serialize_msg.insert(serialize_msg.end(), size_bites.begin(), size_bites.end());
    serialize_msg.insert(serialize_msg.end(), msg.begin(), msg.end());

    std::cout << "\n" << std::endl;
    return serialize_msg;
}

std::string client::deserialize_from_buf()
{
    std::cout << "\n" << std::endl;
    std::cout << "DESERIALIZE" << std::endl;
    boost::asio::streambuf::const_buffers_type bufs = _buf.data();

    
    if (boost::asio::buffer_size(bufs) == 0)
        return "EMPTY:(";

    uint64_t message_size = static_cast<uint8_t>(*boost::asio::buffer_cast<const char*>(bufs));
    
    std::string message(boost::asio::buffer_cast<const char*>(bufs) + 1, message_size);

    std::cout << "\n" << std::endl;
    return message;
}

void client::sent_request()
{
    std::cout << "\n" << std::endl;
    std::cout << "SENT REQUEST" << std::endl;

    boost::system::error_code err;

    std::cout << "SERIALIZE MESSAGE" << std::endl;
    std::vector<uint8_t> msg = serialize_in_buf("EGOR PIDOR");

    std::cout << "MESSAGE AFTER SERIALIZE" << std::endl;
    std::cout << std::string(msg.begin(), msg.end()) << std::endl;
    std::cout << std::to_string(msg.size()) << std::endl;

    boost::asio::write(_socket, boost::asio::buffer(msg), err);
    get_response();

    std::cout << "\n" << std::endl;
}

void client::get_response()
{
    std::cout << "\n" << std::endl;
    std::cout << "GET REPONSE" << std::endl;
    boost::system::error_code err;
    boost::asio::read(_socket, _buf, err);
    if(err) 
    {
        std::cout << "ERROR: " << err.message() << std::endl;
        _socket.close();
    }
    else 
    {
        std::cout << "Get: " << deserialize_from_buf() << std::endl;
    }
    std::cout << "\n" << std::endl;
}
