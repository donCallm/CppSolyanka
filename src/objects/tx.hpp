#pragma once

#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <server/state/state.hpp>
#include "user.hpp"

namespace core
{

class state;

class tx
{
    public:
        enum type
        {
            default_tx = 0,
            replenish_balance = 1,
            debit_funds = 2
        };
        tx(state& state_, uint64_t id_, std::vector<std::string> data_, user usr, type tx_type_ = default_tx);
        operator nlohmann::json() const { return nlohmann::json{ {"id", id}, {"tx_type", tx_type}, {"data", data} }; }

    public:
        bool validate();
        void executing();
        void from_json(const nlohmann::json& json_data);

    private:
        state& _state;

    public:
        uint64_t id;
        type tx_type;
        std::vector<std::string> data;
        user usr;

};

}
