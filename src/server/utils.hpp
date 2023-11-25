#pragma once

#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>

namespace utils
{
    constexpr const char TOKEN[] = "107610801084107232108310861093";

    template <typename T>
    inline std::string to_str(const std::string& msg)
    {
        T message(msg);
        nlohmann::json json_message = message;
        return json_message.dump();
    }

    inline std::string to_str(const std::unordered_map<uint64_t, std::string>& map)
    {
        nlohmann::json json;

        for (const auto& element : map)
        {
            json[std::to_string(element.first)] = element.second;
        }

        return json.dump();
    }
}