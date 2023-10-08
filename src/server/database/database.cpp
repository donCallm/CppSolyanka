#include "database.hpp"
#include <iostream>
#include <future>

namespace db
{
    database* database::_db{nullptr};
    std::mutex database::_mtx;

    database::database()
    {
        _redis.connect("127.0.0.1", 6379);
    }

    database* database::get_instance()
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_db == nullptr) _db = new database();
        return _db;
    }

    void database::select_db(db_list db_name)
    {
        _redis.select(db_name);
    }
    
    void database::write(db_list db_name, std::string key, std::string to_write)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        select_db(db_name);
        _redis.set(key, to_write);
        _redis.sync_commit();
    }
    
    std::string database::read(db_list db_name, std::string key)
    {
        std::lock_guard<std::mutex> lock(_mtx);
        select_db(db_name);
        std::vector<std::string> vecKeys = {"0"};

    _redis.del(vecKeys, [key](cpp_redis::reply& reply) {
        if (reply.is_integer()) {
            std::cout << "Key deleted successfully." << std::endl;
        } else {
            std::cout << "Failed to delete key." << std::endl;
        }
    });
        std::promise<std::string> result_promise;
        auto res = result_promise.get_future();

        _redis.get(key, [&result_promise](const cpp_redis::reply& reply){
            if (reply.is_null()) result_promise.set_value("empty");
            else result_promise.set_value(reply.as_string());
        });
        _redis.sync_commit();
        return res.get();
    }
}