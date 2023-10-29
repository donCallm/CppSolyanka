#pragma once

#include <cpp_redis/cpp_redis>

namespace db 
{
    class database {
            
        private:
            database();
            database(const database&) = delete;
            database& operator=(const database&) = delete;

        public:
            enum db_list
            {
                clients_info = 0,
                transaction = 1,
                last_id = 2
            };

        public:
            static database* get_instance();
            void write(const db_list& db_name, const std::string& key, const std::string& to_write);
            std::string read(const db_list& db_name, const std::string& key);

        private:
            void select_db(const db_list& db_name);

        private:
            static db::database* _db;
            static std::mutex _mtx;
            cpp_redis::redis_client _redis;
    };
}
