#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class error
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"error_msg", error_msg} }; };
            
        public:
            std::string get_json();
            void from_json(const nlohmann::json& json_data);

        public:
            std::string error_msg;
    };
}
