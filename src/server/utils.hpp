#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <regex>

namespace utils
{
    inline bool is_number(const std::string& str)
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isdigit(c); });
    }

    inline bool is_valid_str(const std::string& input, const std::string& pattern_)
    {
        std::regex pattern(pattern_);
        return std::regex_match(input, pattern);
    }

    inline bool validate_params(const std::vector<std::string>& params, const uint64_t &number_of_params)
    {
        if (number_of_params != params.size())
            return false;
        return true;
    }

    inline bool validate_params(const std::vector<std::string>& params, const std::string& pattern)
    {
        for (size_t i = 0; i < params.size(); ++i)
        {
            if (!is_valid_str(params[i], pattern))
                return false;
        }
        return true;
    }
}