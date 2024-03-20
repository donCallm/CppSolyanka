#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include <regex>

namespace utils
{
    constexpr const char TOKEN[] =              "bf4cefbfeeaedaaefbd9af4decbbebdb";
    // RGX_NUMS" - checking for missing numbers in a string
    constexpr const char RGX_NUMS[] =            "^[0-9]+$";
    // RGX_LTRS_NUMS - checking for missing numbers/letters in a string
    constexpr const char RGX_LTRS_NUMS[] =       "^[a-zA-Z0-9]+$";
    
    constexpr const char LAST_CARD_ID[] =       "last_card_id";
    constexpr const char LAST_BANK_ACC_ID[] =   "last_bank_acc_id";
    constexpr const char LAST_USER_ID[] =       "last_user_id";
    constexpr const char LAST_TX_ID[] =         "last_tx_id";
    
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