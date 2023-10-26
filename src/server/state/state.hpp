#pragma once

#include <objects/commands.hpp>
#include <objects/user.hpp>
#include <server/database/database.hpp>
#include <optional>
#include <mutex>

namespace core
{
    class state
    {   
    public:
        state();

    private:
        bool client_exist(std::string& pasport);

    public:
        uint64_t get_new_id();
        
        bool create_user(core::user& client);
        std::optional<user> get_user(std::string& parport);
        void setup();

    private:
        std::atomic<uint64_t> _last_user_id;
        db::database* _db;
        std::mutex _m;
    };
}
