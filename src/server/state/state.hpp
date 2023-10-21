#pragma once

#include "commands.hpp"
#include "user.hpp"
#include "database.hpp"
#include "reply.hpp"
using namespace core;

namespace server
{
    class state
    {   
        public:
            state();

        private:
            bool client_exist(std::string& pasport);

        public:
            reply registration(core::commands& comm, core::user& client);
            reply login(core::commands& comm, core::user& client);
            user get_user(std::string& parport);
            void initialize();

        private:
            uint64_t _last_user_id;
            db::database* _db;
    };
}
