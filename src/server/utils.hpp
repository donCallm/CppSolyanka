#pragma once

#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <regex>

namespace utils
{
    constexpr const char TOKEN[] = "107610801084107232108310861093";

    template <typename T>
    inline std::string to_str(const std::string& msg, const std::vector<int>& prms = {})
    {
        T message(msg);
        nlohmann::json json_message = message;
        return json_message.dump();
    }

    inline std::string to_str(const std::unordered_map<std::string, uint64_t>& map)
    {
        nlohmann::json json;

        for (const auto& element : map)
        {
            json[element.first] = element.second;
        }

        return json.dump();
    }

    inline bool is_number(const std::string& str)
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isdigit(c); });
    }

    inline bool is_valid_str(const std::string& input)
    {
        std::regex pattern("^[a-zA-Z0-9]+$");
        return std::regex_match(input, pattern);
    }
}