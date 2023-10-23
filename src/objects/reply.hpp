#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class reply
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"msg", msg}, {"id", id} }; }
            reply();

        public:
            void from_json(const nlohmann::json& json_data);

        public:
            static uint64_t id;
            std::string msg;
    };
}
