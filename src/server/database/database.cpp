#include <server/database/database.hpp>
#include <iostream>
#include <future>
#include <spdlog/spdlog.h>

namespace db
{
    database* database::_db{nullptr};
    std::mutex database::_mtx;

    database::database()
    {
        _redis.connect(IP, PORT);
        spdlog::info("Create database at {}:{}", IP, PORT);
    }

    database* database::get_instance()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_db == nullptr) 
            _db = new database();
        return _db;
    }

    void database::select_db(const db_list& db_name)
    {
        _redis.select(db_name);
    }
    
    void database::write(const db_list& db_name, const std::string& key, const std::string& to_write)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        select_db(db_name);
        _redis.set(key, to_write);
        _redis.sync_commit();
    }
    
    std::string database::read(const db_list& db_name, const std::string& key)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        select_db(db_name);
        std::promise<std::string> result_promise;

        auto res = result_promise.get_future();
        _redis.get(key, [&result_promise](const cpp_redis::reply& reply){
            if (reply.is_null()) 
                result_promise.set_value("");
            else 
                result_promise.set_value(reply.as_string());
        });
        _redis.sync_commit();
        return res.get();
    }
}