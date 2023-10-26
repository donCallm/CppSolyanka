#pragma once

#include <iostream>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <nlohmann/json.hpp>
#include "user.hpp"

namespace core
{
    class bank_account
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"pan", pan}, {"holder_name", holder_name},
                {"holder_passport", holder_passport}, {"balance", balance}, {"transactions", transactions} }; }

        public:
            void from_json(const nlohmann::json& json_data);

        public:
            uint64_t pan;
            std::string holder_name;
            std::string holder_passport;
            int balance;
            std::vector<transaction> transactions;
    };

    class transaction
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"pan", pan},
                {"client_pasport", client_pasport}, {"info", info} }; }
            enum type
            {
                transfers = 0,
                deposit = 1,
                withdrawal = 2,
                balance_inquiry = 3,
                statement_request = 4
            };

            public:
                void from_json(const nlohmann::json& json_data);

        public:
            uint64_t id;
            std::string client_pasport;
            std::string info;
    };

    class card
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"pan", pan}, {"сvv", сvv},
                {"expiration_date", expiration_date}, {"holder_name", holder_name}, {"linked_acc", linked_acc} }; }

        public:
            void from_json(const nlohmann::json& json_data);

        public:
            uint64_t pan;
            std::string сvv;
            boost::gregorian::date expiration_date;
            std::string holder_name;
            bank_account linked_acc;
    };
}
