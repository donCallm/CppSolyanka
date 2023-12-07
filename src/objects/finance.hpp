#pragma once

#include <iostream>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <nlohmann/json.hpp>
#include <unordered_set>
#include <optional>
#include "user.hpp"

#define MAX_CARDS_COUNT 20
#define MAX_BANK_ACCS_COUNT 20


namespace core
{

class bank_account
{
    public:
        operator nlohmann::json() const {
            return nlohmann::json { 
                {"id", id},
                {"balance", balance},
                {"transactions_id", transactions_id} 
            }; 
        }
        explicit bank_account(uint64_t id_);
        bank_account() {};

    public:
        void from_json(const nlohmann::json& json_data);

    public:
        uint64_t id;
        int balance;
        std::unordered_set<uint64_t> transactions_id;
};


class card
{
    public:
        operator nlohmann::json() const {
            return nlohmann::json{
                {"id", id},
                {"bank_account_id", bank_account_id}
            };
        }
        card(uint64_t id_, uint64_t bank_account_id_);
        card() {};
        
    public:
        void from_json(const nlohmann::json& json_data);

    public:
        uint64_t id;
        uint64_t bank_account_id;
};

class transaction
{
    public:
        operator nlohmann::json() const {
            return nlohmann::json{
                {"id", id},
                {"operation", operation},
                {"start_date", start_date},
                {"completion_date", completion_date},
                {"sum", sum}
            };
        }

        enum tx_type
        {
            debit = 0,
            replenishment = 1
        };
        
        transaction(uint64_t id_, const std::string& start_date_, uint64_t sum_);
        transaction(){}

    public:
        void from_json(const nlohmann::json& json_data);

    public:
        uint64_t id;
        tx_type operation;
        std::string start_date;
        std::string completion_date;
        uint64_t sum;
};

}