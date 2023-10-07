#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class user
    {
        public:
            void from_json(nlohmann::json& json_data);

        public:
            std::string name;
            std::string sername;
            std::string patranomic;
            uint64_t id;
    };
}
