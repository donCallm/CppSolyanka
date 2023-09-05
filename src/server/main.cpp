#include <iostream>
#include <fstream>
#include "server.hpp"

int main()
{
    try
    {
        boost::asio::io_service io_service;
        boost::asio::io_service::work work(io_service);
        server server(io_service);
        io_service.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
