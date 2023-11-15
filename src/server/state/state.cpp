#include "state.hpp"
#include <objects/finance.hpp>
#include <objects/msg_objects.hpp>
#include <spdlog/spdlog.h>
#include <server/database/database.hpp>

using namespace db;

namespace core
{
    state::state() : 
        _last_user_id(0),
        _last_bank_acc_id(0)
    {
        spdlog::info("Start state");
        setup();
    }

    uint64_t state::get_new_id(identifier_type type, const std::string key)
    { 
        std::lock_guard<std::mutex> _(_m);
        uint64_t tmp;

        switch (type)
        {
            case identifier_type::last_user_id: {
                tmp = _last_user_id.fetch_add(1);
                break; }
            case identifier_type::last_bank_acc_id: {
                tmp = _last_bank_acc_id.fetch_add(1);
                break; }
            default:
                break;
        }

        DB()->write(database::last_id, key, std::to_string(tmp));
        return tmp; 
    }

    bool state::create_user(user& usr)
    {
        if (get_user(usr.pasport).has_value())
            return false;

        if (std::find(_active_users.begin(), _active_users.end(), usr.id) != _active_users.end())
            return false;

        usr.id = get_new_id(last_user_id, "last_user_id");

        nlohmann::json json_usr = usr;
        DB()->write(database::clients_info, usr.pasport, json_usr.dump());
        return true;
    }

    bool state::create_bank_account(user& usr)
    {
        if (usr.empty())
            return false;

        bank_account new_acc(get_new_id(last_bank_acc_id, "last_bank_acc_id"));
        nlohmann::json json_acc = new_acc;
        DB()->write(database::bank_accounts, std::to_string(new_acc.id), json_acc.dump());

        usr.bank_accounts.insert(new_acc.id);
        nlohmann::json json_usr = usr;
        DB()->write(database::clients_info, usr.pasport, json_usr.dump());

        return true;
    }

    bool state::login(const std::string& pasport, const std::string& password, user& client)
    {
        std::optional<user> res = get_user(pasport);

        if (res.has_value())
        {
            core::user usr = res.value();
            if (_active_users.find(usr.id) != _active_users.end())
                return false;

            if (usr.password == password)
            {
                client.id = usr.id;
                _active_users.insert(usr.id);
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

    void state::set_value(const std::string& value, identifier_type type)
    {
        if (value.empty())
        {
            switch (type)
            {
                case identifier_type::last_user_id: {
                    DB()->write(database::last_id, "last_user_id", std::to_string(_last_user_id));
                    break; }
                case identifier_type::last_bank_acc_id: {
                    DB()->write(database::last_id, "last_bank_acc_id", std::to_string(_last_bank_acc_id));
                    break; }
                default:
                    break;
            }
        }
        else
        {
            switch (type)
            {
                case identifier_type::last_user_id: {
                    _last_user_id = std::stoull(value);
                    break; }
                case identifier_type::last_bank_acc_id: {
                    _last_bank_acc_id = std::stoull(value);
                    break; }
                default:
                    break;
            }
        }
    }

    void state::setup()
    {
        set_value(DB()->read(database::last_id, "last_user_id"), last_user_id);
        set_value(DB()->read(database::last_id, "last_bank_acc_id"), last_bank_acc_id);
    }
}