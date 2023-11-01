#include "state.hpp"
#include <objects/msg_objects.hpp>
#include <spdlog/spdlog.h>
#include <server/database/database.hpp>

using namespace db;

namespace core
{
    state::state() : 
        _last_user_id(0)
    {
        spdlog::info("Start state");
        setup();
    }

    uint64_t state::get_new_id()
    { 
        std::lock_guard<std::mutex> _(_m);
        uint64_t tmp = _last_user_id.fetch_add(1);
        DB()->write(database::last_id, "last_user_id", std::to_string(tmp));
        return tmp; 
    }

    bool state::create_user(core::user& usr)
    {
        if (get_user(usr.pasport).has_value())
            return false;
        if (std::find(_active_users.begin(), _active_users.end(), usr.id) == _active_users.end())
            return false;

        usr.id = get_new_id();

        nlohmann::json json_usr = usr;
        DB()->write(database::clients_info, usr.pasport, json_usr.dump());
        return true;
    }

    bool state::login(const std::string& pasport, const std::string& password)
    {
        std::optional<user> res  = get_user(pasport);

        if (res.has_value())
        {
            if (std::find(_active_users.begin(), _active_users.end(), res.value().id) == _active_users.end())
                return false;

            core::user usr = res.value();
            if (usr.password == password)
            {
                _active_users.push_back(usr.id);
                return true;
            }
        }

        return false;
    }

    std::optional<user> state::get_user(const std::string& pasport)
    {
        std::string res = DB()->read(database::clients_info, pasport);
        if (res.empty()) 
            return {};

        user usr;
        usr.from_json(nlohmann::json::parse(res));
        return usr;
    }

    void state::setup()
    {
        std::string res = DB()->read(database::last_id, "last_user_id");
        if (!res.empty())
        {
            _last_user_id = std::stoull(res);
        }
        else 
        {
            DB()->write(database::last_id, "last_user_id", std::to_string(_last_user_id));
        }
    }
}