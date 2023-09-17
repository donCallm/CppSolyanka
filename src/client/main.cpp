#include "client.hpp"
#include <spdlog/spdlog.h>

int main()
{
    try
    {
        core::client user;
    }
    catch(const std::exception& e)
    {
        spdlog::error("unhandled exception: {}", e.what());
        return 1; 
    }
    
}