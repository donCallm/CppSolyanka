#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class reply
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"msg", msg}, {"params", params} }; }

        public:
            void from_json(const nlohmann::json& json_data);

        public:
            std::string msg;
            std::vector<std::string> params;
    };
}
