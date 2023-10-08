#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class user
    {
        public:
            void from_json(nlohmann::json& json_data);
            void set_data(std::string string_data);

        public:
            std::string name;
            std::string sername;
            std::string patranomic;
            std::string password;
            uint64_t id;
    };
}
