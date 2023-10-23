#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class success_result
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"result_msg", result_msg} }; }
            success_result();
            success_result(std::string data);

        public:
            std::string get_json();
            void from_json(const nlohmann::json& json_data);

        public:
            std::string result_msg;
    };
}
