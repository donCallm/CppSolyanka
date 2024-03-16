#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <server/state/state.hpp>
#include "user.hpp"

namespace core
{

class user;
class state;

class tx
{
    public:
        enum type
        {
            default_tx = 0,
            tx_send = 1
        };
        tx(uint64_t id_, type tx_type_ = default_tx);
        operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"tx_type", tx_type} }; }
        virtual ~tx() = default;

    public:
        virtual bool validate(core::state& st)                      { return false; }
        virtual void executing(core::state& st)                     { std::cout << "base" << std::endl; }
        virtual void from_json(const nlohmann::json& json_data)     {}

    public:
        uint64_t id;
        type tx_type;
};

class tx_send : public tx
{
    public:
        enum type_of_send
        {
            default_type = 0,
            replenish_balance = 1,
            debit_funds = 2
        };
        tx_send(uint64_t id_, uint64_t sum_, uint64_t sender_id_, uint64_t sender_card_id_,
            uint64_t receiver_card_id_ = 0, uint64_t receiver_id_ = 0, type_of_send type_ = default_type);
        ~tx_send() override = default;
        operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"tx_type", tx_type}, {"snd_type", snd_type}, {"sender_id", sender_id}, {"receiver_id", receiver_id} }; }

    public:
        bool validate(core::state& st) override;
        void executing(core::state& st) override;
        void from_json(const nlohmann::json& json_data) override;

    public:
        type_of_send snd_type;
        uint64_t sum;
        uint64_t sender_id;
        //if the debit or replenish type receiver id and id him card will be zero
        uint64_t receiver_id;
    
    private:
        uint64_t _sender_card_id;
        uint64_t _receiver_card_id;
};

}
