#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace utils
{
    constexpr const char LOGINS[] = "/home/egor/CppSolyanka/src/server/state/logins.json";
    constexpr const char TOKEN[] = "107610801084107232108310861093";
    template <typename T>
    std::string to_str(const std::string& msg)
    {
        T message(msg);
        nlohmann::json json_message = message;
        return json_message.dump();
    }
}