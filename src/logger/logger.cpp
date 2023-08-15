#include "logger.hpp"

void logger::get_info(std::string msg)
{
    if (!msg.empty())
        std::cout << msg << std::endl;
}