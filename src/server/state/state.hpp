#pragma once

#include <objects/commands.hpp>
#include <objects/user.hpp>
#include <atomic>
#include <optional>
#include <mutex>

namespace core
{
    class state
    {   
    public:
        state();

    public:
        uint64_t get_new_id();
        
        bool create_user(core::user& usr);
        std::optional<user> get_user(const std::string& pasport);
        bool login(const std::string& pasport, const std::string& password);

        void setup();

    private:
        std::vector<std::string> _active_users;
        std::atomic<uint64_t> _last_user_id;
        std::mutex _m;
    };
}
