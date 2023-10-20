#pragma once

#include "commands.hpp"
#include "user.hpp"
#include "database.hpp"

namespace server
{
    class state
    {   
        public:
            enum operation_result { wrong_params, wrong_pass, already_exist,
                already_authorized, wrong_pasport, successful_registration, successful_logged };
            state(): _last_user_id(1), _db(db::database::get_instance()) {}

        private:
            bool client_exist(std::string& pasport);

        public:
            operation_result registration(core::commands& comm, core::user& client);
            operation_result login(core::commands& comm, core::user& client);
            core::user get_user(std::string& parport);
            void initialize();

        private:
            uint64_t _last_user_id;
            db::database* _db;
    };
}
