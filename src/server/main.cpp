#include "app.hpp"
#include <spdlog/spdlog.h>

int main()
{
    try
    {
        boost::asio::io_service io_service;
        boost::asio::io_service::work work(io_service);
        core::app application(io_service);
        io_service.run();
    }
    catch(const std::exception& e)
    {
        spdlog::error("unhandled exception: {}", e.what());
        return 1; 
    }
    catch(...)
    {
        spdlog::error("unhandled exception");
        return 1; 
    }
    
    return 0;
}
