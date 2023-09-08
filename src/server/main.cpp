#include "server.hpp"
#include <spdlog/spdlog.h>

int main()
{
    try
    {
        boost::asio::io_service io_service;
        boost::asio::io_service::work work(io_service);
        core::server server(io_service);
        io_service.run();
    }
    catch(const std::exception& e)
    {
        spdlog::error("unhandled exception: {}", e.what());
        return 1; 
    }
    
    return 0;
}
