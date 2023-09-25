#include "database.hpp"
#include <iostream>
#include <future>

namespace db
{
    database::database()
    {
        _redis.connect("127.0.0.1", 6379);
    }

    database& database::instance()
    {
        static database instance;
        return instance;
    }

    void database::select_db(db_list db_name)
    {
        _redis.select(db_name);
    }
    
    void database::write(db_list db_name, std::string key, std::string to_write)
    {
        db::database& db = database::instance();
        db.select_db(db_name);
        db._redis.set(key, to_write);
        db._redis.sync_commit();
    }
    
    std::string database::read(db_list db_name, int key)
    {
        db::database& db = database::instance();
        db.select_db(db_name);

        std::promise<std::string> result_promise;
        auto res = result_promise.get_future();

        db._redis.get(std::to_string(key), [&result_promise](const cpp_redis::reply& reply){
            result_promise.set_value(reply.as_string());
        });

        db._redis.sync_commit();

        return res.get();
    }
}