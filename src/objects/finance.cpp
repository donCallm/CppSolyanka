#include "finance.hpp"

namespace core
{
    
void bank_account::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("pan").get_to(pan);
    json_data.at("holder_name").get_to(holder_name);
    json_data.at("holder_passport").get_to(holder_passport);
    json_data.at("balance").get_to(balance);
    json_data.at("transactions").get_to(transactions);
}

void transaction::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("id").get_to(id);
    json_data.at("client_pasport").get_to(client_pasport);
    json_data.at("info").get_to(info);
}

void card::from_json(const nlohmann::json& json_data)
{
    if (json_data.empty()) throw std::runtime_error("Empty json");

    json_data.at("pan").get_to(pan);
    json_data.at("сvv").get_to(сvv);
    json_data.at("expiration_date").get_to(expiration_date);
    json_data.at("holder_name").get_to(holder_name);
    json_data.at("linked_acc").get_to(linked_acc);
}

}
