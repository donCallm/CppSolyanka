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

void bank_account::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("id").get_to(id);
    json_data.at("balance").get_to(balance);
}

void card::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("id").get_to(id);
    json_data.at("bank_account_id").get_to(bank_account_id);
}

}
