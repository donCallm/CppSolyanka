#include "connect.hpp"

boost::asio::ip::tcp::socket& con_handler::get_socket()
{
    return sock;
} 

std::vector<std::string> con_handler::split_string(const std::string& input)
{ 
    logger::get_info("SPLIT STRING");
    std::vector<std::string> result;

    if (input.empty())
    {
        logger::get_info("EMPTY :(");
        return result;
    }

    std::istringstream iss(input);
    std::string token;

    while (std::getline(iss, token, ' ')) {
        result.push_back(token);
    }
    
    return result;
}

void con_handler::read_message(std::function<void(std::string)> callback)
{
    logger::get_info("READ MESSAGE");

    boost::asio::async_read(sock,
        boost::asio::buffer(_read_size),
        [self = shared_from_this(), callback](const boost::system::error_code& error, size_t bytes_transferred) {
            if (!error)
            {
                uint64_t received_value;
                std::memcpy(&received_value, self->_read_size.data(), sizeof(uint64_t));
                self->_recv_msg.resize(received_value);

                boost::asio::async_read(self->sock,
                    boost::asio::buffer(self->_recv_msg),
                    [self, callback](const boost::system::error_code& error, size_t bytes_transferred) {
                        if (!error)
                        {
                            std::string message(self->_recv_msg.begin(), self->_recv_msg.end());
                            callback(message);
                        }
                        else
                        {
                            std::cerr << "error: " << error.message() << std::endl;
                            self->sock.close();
                            callback(""); 
                        }
                    });
            }
            else
            {
                logger::get_info("HANDLE RAD");
                std::cerr << "error: " << error.message() << std::endl;
                self->sock.close();
                callback("");
            }
        });
}

void con_handler::start()
{
    logger::get_info("START");

    read_message([this](const std::string& message) {
        if (!message.empty()) 
        {
            logger::get_info("Received message from server: " + message);
            this->write_message("pong");
            this->start();
        } 
        else 
        {
            logger::get_info("Received empty message or error from server");
        }
    });

}

std::vector<uint8_t> con_handler::serialize(const std::string msg)
{
    logger::get_info("SERIALIZE");

    std::vector<uint8_t> serialized_msg;

    if (msg.empty())
    {
        logger::get_info("EMPTY :(");
        return serialized_msg;
    }
    
    uint64_t msg_size = msg.size();

    std::vector<uint8_t> size_bytes(sizeof(uint64_t));
    for (size_t i = 0; i < sizeof(uint64_t); ++i) 
        size_bytes[i] = (msg_size >> (i * 8)) & 0xFF;
    
    serialized_msg.insert(serialized_msg.end(), size_bytes.begin(), size_bytes.end());
    serialized_msg.insert(serialized_msg.end(), msg.begin(), msg.end());

    return serialized_msg;
}

void con_handler::write_message(std::string message)
{ 
    logger::get_info("WRITE");
    auto msg = serialize(message);

   boost::asio::async_write(sock,
        boost::asio::buffer(msg.data(), msg.size()),
        boost::bind(&con_handler::handle_write,
        shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
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


bool con_handler::is_started() const
{
    return _started;
}

void con_handler::set_started(bool value)
{
    _started = value;
}

con_handler::ptr con_handler::create(boost::asio::io_service& io_service)
{
    return ptr(new con_handler(io_service));
}