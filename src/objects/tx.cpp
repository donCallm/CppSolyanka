#include "tx.hpp"
#include <optional>
#include <server/database/database.hpp>

using namespace db;

namespace core
{
    tx::tx(state& state_, uint64_t id_, std::vector<std::string> data_,  user usr_, type tx_type_) :
    _state(state_),
    id(id_),
    tx_type(tx_type_),
    data(data_),
    usr(usr_)
    {}

    void tx::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("id").get_to(id);
        json_data.at("tx_type").get_to(tx_type);
        json_data.at("data").get_to(data);
    }

    bool tx::validate()
    {
        //case because it is based on the fact that new transactions will appear
        switch (tx_type)
        {
            case type::debit_funds :
            case type::replenish_balance :
            {
                auto iter = usr.cards.find(std::stoull(data[0]));//
                if (iter == usr.cards.end())//
                    return false;

                std::optional<card> res_card = _state.get_card(std::stoull(data[0]));
                if(!res_card.has_value())
                    return false;

                std::optional<bank_account> res_bank_acc = _state.get_bank_account(res_card.value().bank_account_id);
                if (!res_bank_acc.has_value())
                    return false;

                if (tx_type == type::debit_funds)
                {
                    if (std::stoull(data[1]) > res_bank_acc.value().balance)
                        return false;
                }

                return true;
            }
            default:
                return false;
        }
    }

    void tx::executing()
    {
        switch (tx_type)
        {
            case type::debit_funds :
            case type::replenish_balance :
            {
                std::optional<card> res_card = _state.get_card(std::stoull(data[0]));
                std::optional<bank_account> res_bank_acc = _state.get_bank_account(res_card.value().bank_account_id);
                bank_account acc = res_bank_acc.value();

                if (tx_type == type::debit_funds)
                    acc.balance -= std::stoull(data[1]);
                else
                    acc.balance += std::stoull(data[1]);

                nlohmann::json json_acc = acc;
                DB()->write(database::bank_accounts, std::to_string(acc.id), json_acc.dump());
                break;
            }
            default:
                break;
        }
    }
}