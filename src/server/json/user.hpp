#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace for_json
{
    class user
    {
        public:
            void from_json(nlohmann::json& json_data)
            {
                json_data.at("name").get_to(name);
                json_data.at("sername").get_to(sername);
                json_data.at("patranomic").get_to(patranomic);
                json_data.at("id").get_to(id);
            }

        public:
            std::string name;
            std::string sername;
            std::string patranomic;
            int id;
    };
}