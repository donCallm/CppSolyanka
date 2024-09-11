#pragma once

#include <iostream>

namespace core
{
    class msg
    {
    public:
        explicit msg(std::string data);
        msg();
        ~msg(){}
    
    public:
        void set_message(const std::string& data);

    public:
        static uint64_t id;
        std::string message;
    };

}