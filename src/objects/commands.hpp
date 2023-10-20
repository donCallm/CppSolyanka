#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class commands
    {
        public:
            enum type { ping, registration, login, end, uncknow };
            operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"instruction", instruction}, {"params", params}, {"token", token} }; }

        public:
            void from_json(const nlohmann::json& json_data);
            void set_command(const std::string& input);

        public:
            const std::unordered_map<std::string, type> command_map = {
                {"ping", type::ping},
                {"login", type::login},
                {"registration", type::registration},
                {"end", type::end}
            };
            type instruction;
            std::vector<std::string> params;
            std::string token;
            static uint64_t id;
    };
}
