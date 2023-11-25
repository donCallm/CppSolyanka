#include "finance.hpp"

namespace core
{

bank_account::bank_account(uint64_t new_id) : 
    id(new_id),
    balance(0)
{}

void bank_account::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("id").get_to(id);
    json_data.at("balance").get_to(balance);
    json_data.at("transactions_id").get_to(transactions_id);
}

}
