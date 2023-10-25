#pragma once

#include "commands.hpp"
#include "user.hpp"
#include "database.hpp"
#include "user.hpp"
using namespace core;

namespace server
{
    class state
    {   
        public:
            state();

        private:
            bool client_exist(const std::string& pasport);

        public:
            std::string registration(const core::commands& comm, const core::user& client);
            std::string login(const core::commands& comm, core::user& client);
            user get_user(const std::string& parport);
            void setup();

        private:
            uint64_t _last_user_id;
            db::database* _db;
    };
}
