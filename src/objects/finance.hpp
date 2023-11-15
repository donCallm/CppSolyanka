#pragma once

#include <iostream>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include "user.hpp"

namespace core
{

class bank_account
{
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"balance", balance}, {"transactions_id", transactions_id} }; }
        bank_account(uint64_t new_id);

    public:
        void from_json(const nlohmann::json& json_data);

    public:
        uint64_t id;
        int balance;
        std::unordered_set<uint64_t> transactions_id;
};

}