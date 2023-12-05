#include "finance.hpp"
#include <server/database/database.hpp>

namespace core
{

bank_account::bank_account(uint64_t id_) : 
    id(id_),
    balance(0)
{}

card::card(uint64_t id_, uint64_t bank_account_id_) :
    id(id_),
    bank_account_id(bank_account_id_)
{}

transaction::transaction(uint64_t id_, const std::string& start_date_, uint64_t sum_) :
    id(id_),
    start_date(start_date_),
    sum(sum_)
{}

void bank_account::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("id").get_to(id);
    json_data.at("balance").get_to(balance);
    json_data.at("transactions_id").get_to(transactions_id);
}

void card::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("id").get_to(id);
    json_data.at("bank_account_id").get_to(bank_account_id);
}

void transaction::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("id").get_to(id);
    json_data.at("operation").get_to(operation);
    json_data.at("start_date").get_to(start_date);
    json_data.at("start_date").get_to(start_date);
    json_data.at("sum").get_to(sum);
}

}
