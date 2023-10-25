#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class commands
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"instruction", instruction}, {"params", params}, {"token", token} }; }
            enum type {
                        ping = 0,
                        registration = 1,
                        login = 2,
                        end = 3,
                        help = 4,
                        unknown_command = 5
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
