#pragma once

#include "commands.hpp"
#include "user.hpp"
#include "database.hpp"

namespace server_state
{
    class state
    {   
        private:
            static bool client_exist(uint64_t user_id);
            static bool valid_number_of_params(uint16_t correct_numb, core::commands& comm);

        public:
            static std::string registration(core::commands& comm);
            static core::user get_user(uint64_t user_id);
            static void set_state();

        private:
            static uint64_t _last_user_id;
            static db::database* _db;
    };
}
