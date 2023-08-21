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
        boost::asio::io_service::work work(io_service);
        logger::get_info("AFTER WORK(MAIN)");
        server server(io_service);
        io_service.run();
        logger::get_info("AFTER RUN(MAIN)");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
