#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class command
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"instruction", instruction}, {"params", params}, {"token", token} }; }
        enum type
        {
            ping = 0,
            registration = 1,
            login = 2,
            create_bank_acc = 3,
            get_cards = 4,
            get_balance = 5,
            replenish_balance = 6,
            debit_funds = 7,
            get_bank_accounts = 8,
            create_card = 9,
            get_info = 10,
            end = 11,
            unknown_command = 12
        };

    public:
        void from_json(const nlohmann::json& json_data);
        void set_command(const std::string& input);

    public:
        static const std::unordered_map<std::string, type> command_map;
        type instruction;
        std::vector<std::string> params;
        std::string token;
        static uint64_t id;
    };
}
