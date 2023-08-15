#include <iostream>
#include <fstream>
#include "server.hpp"
#include <nlohmann/json.hpp>

int main()
{
    try
    {
        logger::get_info("Start");
        boost::asio::io_service io_service;
        server server(io_service);
        io_service.run();
        logger::get_info("Wait connection");
        io_service.stop();
        logger::get_info("Stop");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
