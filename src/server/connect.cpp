#include "connect.hpp"

boost::asio::ip::tcp::socket& con_handler::get_socket()
{
    return sock;
} 

std::vector<uint8_t> con_handler::serialize_in_buf(const std::string msg)
{
    uint64_t msg_size = msg.size();

    std::vector<uint8_t> size_bytes(sizeof(uint64_t));
    for (size_t i = 0; i < sizeof(uint64_t); ++i) 
        size_bytes[i] = (msg_size >> (i * 8)) & 0xFF;

    std::vector<uint8_t> serialized_msg;
    serialized_msg.insert(serialized_msg.end(), size_bytes.begin(), size_bytes.end());
    serialized_msg.insert(serialized_msg.end(), msg.begin(), msg.end());

    return serialized_msg;
}

std::string con_handler::deserialize_from_buf()
{

    boost::asio::streambuf::const_buffers_type bufs = _buf.data();
    
    if (boost::asio::buffer_size(bufs) == 0)
        return "EMPTY:(";

    uint8_t message_size = static_cast<uint8_t>(*boost::asio::buffer_cast<const char*>(bufs));
    
    std::string message(boost::asio::buffer_cast<const char*>(bufs) + 1, message_size);
    return message;
}

void con_handler::start()
{
    boost::asio::async_read(sock,
            boost::asio::buffer(_read_size),
            [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
                if (!error)
                {
                    uint64_t received_value;
                    std::memcpy(&received_value, self->_read_size.data(), sizeof(uint64_t));
                    std::cout << "Received uint64_t msg size: " << received_value << std::endl;

                    self->_recv_msg.resize(received_value);

                    boost::asio::async_read(self->sock,
                        boost::asio::buffer(self->_recv_msg),
                        [self](const boost::system::error_code& error, size_t bytes_transferred) {
                            if (!error)
                            {
                                std::cout << "Received MESSAGE: " << std::string(self->_recv_msg.begin(), self->_recv_msg.end());
                                std::cout << std::endl;
                            }
                            else
                            {
                                std::cerr << "error: " << error.message() << std::endl;
                                self->sock.close();
                            }
                        });

                }
                else
                {
                    logger::get_info("HANDLE RAD");
                    std::cerr << "error: " << error.message() << std::endl;
                    self->sock.close();
                }
            });
            
    auto msg = con_handler::serialize_in_buf("DAROVA ZAEBAL");

    boost::asio::async_write(sock,
        boost::asio::buffer(msg.data(), msg.size()),
        boost::bind(&con_handler::handle_write,
        shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

con_handler::ptr con_handler::create(boost::asio::io_service& io_service)
{
    return ptr(new con_handler(io_service));
}

void con_handler::handle_read(const boost::system::error_code& err, size_t byte_transferred)
{
    if (!err)
    {
        std::cout << "Server Response: "<< deserialize_from_buf() << std::endl;
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
