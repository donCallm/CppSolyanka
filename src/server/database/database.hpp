#pragma once

#include <cpp_redis/cpp_redis>

namespace db
{
    class database {

        public:
            enum db_list{ clients_info = 0, transaction = 1 };

        public:
            static database& instance();
            static void write(db_list db_name, std::string key, std::string to_write);
            static std::string read(db_list db_name, int key);

        private:
            cpp_redis::client _redis;

        private:
            void select_db(db_list db_name);

        private:
            database() 
            {
                _redis.connect("127.0.0.1", 6379);
            }

            database(const database&) = delete;
            database& operator=(const database&) = delete;
    };
}
