#include "state.hpp"
#include <objects/msg_objects.hpp>
#include <spdlog/spdlog.h>

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
        _db->write(database::last_id, "last_user_id", std::to_string(tmp));
        return tmp; 
    }

    bool state::create_user(core::user& usr)
    {
        usr.id = get_new_id();

        nlohmann::json json_usr = usr; 
        _db->write(database::clients_info, usr.pasport, json_usr.dump());
        return true;
    }

    std::optional<user> state::get_user(std::string& pasport)
    {
        std::string res = _db->read(database::clients_info, pasport);
        if (res.empty()) 
            return {};

        user usr;
        usr.from_json(nlohmann::json::parse(res));
        return usr;
    }

    void state::setup()
    {
        std::string res = _db->read(database::last_id, "last_user_id");
        if (!res.empty())
            _last_user_id = std::stoull(res);
        else 
            _db->write(database::last_id, "last_user_id", std::to_string(_last_user_id));
    }
}