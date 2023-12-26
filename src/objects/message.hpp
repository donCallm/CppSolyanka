#pragma once

#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>

namespace core
{
    struct header
    {
        std::size_t msg_size;
    };

    struct message
    {
        std::string data;
    };

    std::vector<uint8_t> serialize_message(const message& msg);
    message deserialize_message(const std::vector<uint8_t>& msg_buff, std::size_t size);
}