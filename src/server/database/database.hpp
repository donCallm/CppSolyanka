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
            enum db_list{ clients_info = 0, transaction = 1 };

        public:
            database* get_instance();
            void write(db_list db_name, std::string key, std::string to_write);
            std::string read(db_list db_name, int key);

        private:
            void select_db(db_list db_name);

        private:
            static db::database*  _db;
            static std::mutex _mtx;
            cpp_redis::redis_client _redis;
    };
}
