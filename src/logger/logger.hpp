#pragma once

#include <iostream>
#include <string>

class logger
{
private:
    logger();
    logger(const logger&) = delete;
    logger& operator=(const logger&) = delete;
public:
    static void get_info(std::string msg);
};
