#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace for_json
{
    class command
    {
        public:
            void from_json(nlohmann::json& json_data)
            {
                json_data.at("comm").get_to(comm);
                json_data.at("params").get_to(params);
            }

        public:
            std::string comm;
            std::vector<std::string> params;
        
        operator nlohmann::json() const { return nlohmann::json{ {"comm", comm}, {"params", params}}; }
    };
}