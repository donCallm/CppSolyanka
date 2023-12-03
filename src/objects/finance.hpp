#pragma once

#include <iostream>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include <optional>
#include "user.hpp"

#define MAX_CARDS_COUNT 20
#define NAX_BANK_ACCS_COUNT 20

namespace core
{

class bank_account
{
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"balance", balance}, {"transactions_id", transactions_id} }; }
        explicit bank_account(uint64_t id_);
        bank_account() {};

    public:
        void bank_acc_from_json(const nlohmann::json& json_data);

    public:
        uint64_t id;
        int balance;
        std::unordered_set<uint64_t> transactions_id;
};


class card
{
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"bank_account_id", bank_account_id} }; }
        card(uint64_t id_, uint64_t bank_account_id_);
        card() {};
        
    public:
        void card_from_json(const nlohmann::json& json_data);

    public:
        uint64_t id;
        uint64_t bank_account_id;
};

}