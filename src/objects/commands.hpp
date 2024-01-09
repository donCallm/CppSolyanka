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
            balace = 4,
            get_cards = 5,
            get_balance = 6,
            replenish_balance = 7,
            debit_funds = 8,
            get_bank_accounts = 9,
            create_card = 10,
            get_info = 11,
            end = 12,
            clear_databases = 13,
            unknown_command = 14
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
