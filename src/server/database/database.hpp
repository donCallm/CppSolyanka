#pragma once

#include <cpp_redis/cpp_redis>

#define DB() (database::get_instance())

namespace db 
{
    class database
    {
        private:
            database();
            database(const database&) = delete;
            database& operator=(const database&) = delete;

        public:
            enum db_list
            {
                clients_info = 0,
                transaction = 1,
                last_id = 2,
                bank_accounts = 3,
                cards = 4,
                clients_id = 5
            };

        public:
            static database* get_instance();
            void write(const db_list& db_name, const std::string& key, const std::string& to_write);
            std::string read(const db_list& db_name, const std::string& key);
            bool clear_databases();

        private:
            void select_db(const db_list& db_name);

        private:
            static db::database* _db;
            static std::mutex _mtx;
            cpp_redis::redis_client _redis;
            static constexpr const char IP[] = "127.0.0.1";
            static constexpr const uint16_t PORT = 6379;
    };
}
