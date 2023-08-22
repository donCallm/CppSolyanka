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

std::vector<uint8_t> client::serialize_in_buf(std::string msg)
{
    uint64_t msg_size = msg.size();
    std::vector<uint8_t> size_bites(sizeof(uint64_t));
    for (size_t i = 0; i < sizeof(uint64_t); ++i) 
        size_bites[i] = (msg_size >> (i * 8)) & 0xFF;

    std::vector<uint8_t> serialize_msg;
    serialize_msg.insert(serialize_msg.end(), size_bites.begin(), size_bites.end());
    serialize_msg.insert(serialize_msg.end(), msg.begin(), msg.end());

    return serialize_msg;
}

std::string client::deserialize_from_buf()
{
    boost::asio::streambuf::const_buffers_type bufs = _buf.data();
    
    if (boost::asio::buffer_size(bufs) == 0)
        return "EMPTY:(";

    uint64_t message_size = static_cast<uint8_t>(*boost::asio::buffer_cast<const char*>(bufs));
    
    std::string message(boost::asio::buffer_cast<const char*>(bufs) + 1, message_size);

    return message;
}

void client::sent_request()
{
    boost::system::error_code err;
    std::vector<uint8_t> msg = serialize_in_buf("EGOR PIDOR");
    boost::asio::write(_socket, boost::asio::buffer(msg), err);
    get_response();
}

void client::get_response()
{
    try
    {
        uint64_t received_value;
        boost::asio::read(_socket, boost::asio::buffer(_read_size));
        std::memcpy(&received_value, _read_size.data(), sizeof(uint64_t));
        std::cout << "Received uint64_t msg size: " << received_value << std::endl;

        _recv_msg.resize(received_value);
        boost::asio::read(_socket, boost::asio::buffer(_recv_msg));

        std::cout << "Received MESSAGE: " << std::string(_recv_msg.begin(), _recv_msg.end());
        std::cout << std::endl;
    }
    catch (const boost::system::system_error& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        _socket.close();
    }
}
