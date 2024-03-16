#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_set>

namespace core
{
    class user
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"name", name}, {"surname", surname}, {"patronymic", patronymic},
                {"password", password}, {"pasport", pasport}, {"id", id}, {"login", login}, {"bank_accounts", bank_accounts},
                {"cards", cards} }; }
            user() {}
            user(std::string& login_, std::string& name_, std::string& surname_, std::string& patronymic_,
                std::string& pasport_, std::string& password_);

        public:
            void from_json(const nlohmann::json& json_data);

            bool empty();

        public:
            std::string login;
            std::string name;
            std::string surname;
            std::string patronymic;
            std::string password;
            std::string pasport;
            uint64_t id;
            std::unordered_set<uint64_t> bank_accounts;
            std::unordered_set<uint64_t> cards;
    };
}
