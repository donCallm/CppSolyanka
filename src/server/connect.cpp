#include "connect.hpp"

boost::asio::ip::tcp::socket& con_handler::get_socket()
{
    return sock;
} 

std::vector<uint8_t> con_handler::serialize_in_buf(const std::string msg)
{
    logger::get_info("\n");
    logger::get_info("IN SERIALIZE");
    uint64_t msg_size = msg.size();

    std::vector<uint8_t> size_bytes(sizeof(uint64_t));
    for (size_t i = 0; i < sizeof(uint64_t); ++i) 
        size_bytes[i] = (msg_size >> (i * 8)) & 0xFF;

    std::vector<uint8_t> serialized_msg;
    serialized_msg.insert(serialized_msg.end(), size_bytes.begin(), size_bytes.end());
    serialized_msg.insert(serialized_msg.end(), msg.begin(), msg.end());

    logger::get_info("\n");
    return serialized_msg;
}

std::string con_handler::deserialize_from_buf()
{
    logger::get_info("\n");
    logger::get_info("IN DESERIALIZE");

    boost::asio::streambuf::const_buffers_type bufs = _buf.data();
    
    if (boost::asio::buffer_size(bufs) == 0)
        return "EMPTY:(";

    uint8_t message_size = static_cast<uint8_t>(*boost::asio::buffer_cast<const char*>(bufs));
    
    std::string message(boost::asio::buffer_cast<const char*>(bufs) + 1, message_size);

    logger::get_info("\n");
    return message;
}

void con_handler::start()
{
    logger::get_info("\n");
    logger::get_info("FROM CONNECT: ");

    boost::asio::async_read(sock,
        _buf,
        boost::asio::transfer_all(),
        [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
            if (!error)
            {
                logger::get_info("HANDLE READ");
                std::cout << "Server Response: " << self->deserialize_from_buf() << std::endl;
            }
            else
            {
                logger::get_info("HANDLE RAD");
                std::cerr << "error: " << error.message() << std::endl;
                self->sock.close();
            }
        });
        
    logger::get_info("AFTER READ IN CONNECT");

    logger::get_info("SERIALIZE MESSAGE");
    auto msg = con_handler::serialize_in_buf("DAROVA ZAEBAL");
    
    logger::get_info("ELEMs IN SERIALIZE MSG");
    for(size_t i = 0; i < msg.size(); ++i)
        logger::get_info(std::to_string(msg[i]) + " ");

    logger::get_info("MESSAGE AFTER SERIALIZE: ");
    std::cout << std::string(msg.begin(), msg.end()) << std::endl;
    std::cout << std::to_string(msg.size()) << std::endl;

    boost::asio::async_write(sock,
        boost::asio::buffer(msg.data(), msg.size()),
        boost::bind(&con_handler::handle_write,
        shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));

    logger::get_info("AFTER WRITE IN CONNECT");
    logger::get_info("\n");
}

con_handler::ptr con_handler::create(boost::asio::io_service& io_service)
{
    logger::get_info("\n");
    logger::get_info("CREATE IN CONNECT");
    logger::get_info("\n");
    return ptr(new con_handler(io_service));
}

void con_handler::handle_read(const boost::system::error_code& err, size_t byte_transferred)
{
    logger::get_info("\n");
    logger::get_info("HANDLE READ");

    if (!err)
    {
        std::cout << "Server Response: "<< deserialize_from_buf() << std::endl;
    }
    else
    {
        std::cerr << "error" << err.message() << std::endl;
        sock.close();
    }

    logger::get_info("AFTER HANDLE READ");
    logger::get_info("\n");
}
void con_handler::handle_write(const boost::system::error_code& err, size_t byte_transferred)
{
    logger::get_info("\n");
    logger::get_info("HANDLE WRITE");

    if (!err)
    {
        std::cout << "Server sent something" << std::endl;
    }
    else
    {
        std::cerr << "error" << err.message() << std::endl;
        sock.close();
    }

    logger::get_info("AFTER HANDLE WRITE");
    logger::get_info("\n");
}
