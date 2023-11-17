#pragma once

#include <objects/commands.hpp>
#include <objects/user.hpp>
#include <atomic>
#include <optional>
#include <mutex>
#include <unordered_set>

namespace core
{
    class state
    {   
    public:
        state();
        enum identifier_type
        {
            last_user_id = 0,
            last_bank_acc_id = 1
        };

    public:
        uint64_t get_new_id(const identifier_type& type, const std::string key);
        void set_value(const std::string& value, const identifier_type& type);
        void set_logins();
        
        std::optional<std::string> get_login(const uint64_t& id);
        std::optional<user> get_user(const std::string& login);

        bool create_user(core::user& usr);
        bool login(const std::string& pasport, const std::string& password, user& client);
        bool create_bank_account(user& usr);

        void setup();

    private:
        std::unordered_map<uint64_t, std::string> _logins;
        std::unordered_set<uint64_t> _active_users;
        std::atomic<uint64_t> _last_user_id;
        std::atomic<uint64_t> _last_bank_acc_id;
        std::mutex _m;
    };
}
