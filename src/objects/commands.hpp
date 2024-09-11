#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

namespace core
{
    class command
    {
    public:
        enum type
        {
            ping = 0,
            end = 12,
            unknown_command = 13
        };

    public:
        void set_command(const std::string& input);

    public:
        static const std::unordered_map<std::string, type> command_map;
        type instruction;
        std::vector<std::string> params;
        static uint64_t id;
    };

    static inline command::type to_command(const std::string& input) {
        if (input == "ping") {
            return command::type::ping;
        } else if (input == "end") {
            return command::type::end;
        } else {
            return command::type::unknown_command;
        }
    }
}
