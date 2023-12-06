#pragma once

#include <objects/commands.hpp>
#include <objects/user.hpp>
#include <objects/finance.hpp>
#include <atomic>
#include <optional>
#include <mutex>
#include <unordered_set>
#include <objects/commands.hpp>

namespace core
{
    class bank_account;
    class card;

    class state
    {   
    public:
        state();
        enum identifier_type
        {
            last_user_id = 0,
            last_bank_acc_id = 1,
            last_card_id = 2
        };

    public:
        uint64_t get_new_id(const identifier_type& type, const std::string key);
        void set_value(const std::string& value, const identifier_type& type);
        void set_logins();
        
        std::optional<uint64_t> get_id(const std::string& login);
        std::optional<user> get_user(uint64_t id);
        std::optional<bank_account> get_bank_account(uint64_t acc_id);
        std::optional<card> get_card(uint64_t card_id);

        bool create_user(core::user& usr);
        bool login(uint64_t id, const std::string& password);
        bool create_bank_account(user& usr);
        std::optional<int> get_balance(uint64_t crd);
        bool change_balance(command::type& operation, uint64_t sum, uint64_t card_id);
        void create_card(user& usr, uint64_t bank_account_id);

        void setup();

    private:
        std::unordered_map<std::string, uint64_t> _logins;
        std::unordered_set<uint64_t> _active_users;
        std::atomic<uint64_t> _last_user_id;
        std::atomic<uint64_t> _last_bank_acc_id;
        std::atomic<uint64_t> _last_card_id;
        std::mutex _m;
    };
}
