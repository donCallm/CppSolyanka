#include "tx.hpp"
#include <optional>
#include <server/database/database.hpp>

using namespace db;

namespace core
{
    tx::tx(uint64_t id_, type tx_type_) :
        id(id_),
        tx_type(tx_type_)
    {}

    tx_send::tx_send(uint64_t id_, uint64_t sum_, uint64_t sender_id_, uint64_t sender_card_id_,
        uint64_t receiver_card_id_, uint64_t receiver_id_, type_of_send type_) : 
        tx(id_, type::tx_send),
        snd_type(type_),
        sum(sum_),
        receiver_id(receiver_id_),
        sender_id(sender_id_),
        _sender_card_id(sender_card_id_),
        _receiver_card_id(receiver_card_id_)
    {}

    void tx_send::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("id").get_to(id);
        json_data.at("tx_type").get_to(tx_type);
        json_data.at("snd_type").get_to(snd_type);
        json_data.at("receiver_id").get_to(receiver_id);
        json_data.at("sender_id").get_to(sender_id);
    }

    bool tx_send::validate(core::state& st)
    {
        if (tx_type == default_tx || snd_type == default_type)
            return false;

        auto sender = st.get_user(sender_id);
        if (!sender.has_value())
            return false;
        
        std::optional<user> receiver;
        if (receiver_id != 0)
        {
            receiver = st.get_user(receiver_id);
            if (!receiver.has_value())
                return false;
        }

        auto iter = sender.value().cards.find(_sender_card_id);
        if (iter == sender.value().cards.end())
            return false;

        std::optional<card> res_card = st.get_card(_sender_card_id);
        if(!res_card.has_value())
            return false;

        std::optional<bank_account> res_bank_acc = st.get_bank_account(res_card.value().bank_account_id);
        if (!res_bank_acc.has_value())
            return false;

        if (snd_type == debit_funds)
        {
            if (sum > res_bank_acc.value().balance)
                return false;
        }

        return true;
    }

    void tx_send::executing(core::state& st)
    {
        std::optional<card> res_card = st.get_card(_sender_card_id);
        std::optional<bank_account> res_bank_acc = st.get_bank_account(res_card.value().bank_account_id);
        bank_account acc = res_bank_acc.value();

        if (snd_type == debit_funds)
            acc.balance -= sum;
        else
            acc.balance += sum;

        nlohmann::json json_acc = acc;
        DB()->write(database::bank_accounts, std::to_string(acc.id), json_acc.dump());
    }
}