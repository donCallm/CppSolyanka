#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class user
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"name", name}, {"surname", surname}, {"patronymic", patronymic},
                {"password", password}, {"pasport", pasport}, {"id", id} }; }

        public:
            void from_json(const nlohmann::json& json_data);
            bool is_empty();

        public:
            std::string name;
            std::string surname;
            std::string patronymic;
            std::string password;
            std::string pasport;
            uint64_t id;
    };
}
