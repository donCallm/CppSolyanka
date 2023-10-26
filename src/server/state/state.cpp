#include "state.hpp"
#include <objects/msg_objects.hpp>
using db::database;

namespace server
{
    state::state(): _last_user_id(0), _db(db::database::get_instance()) {}

    std::string state::registration(core::commands& comm, core::user& client)
    {
        core::error_msg err;

        if (!client.is_empty()) 
            err.error_msg =  "already_authorized";
        else if (comm.params.size() != 5)
            err.error_msg = "wrong_params";
        else if (client_exist(comm.params[3]))
            err.error_msg = "already_exist";

        if (!err.error_msg.empty())
        {
            nlohmann::json err_json = err; 
            return err_json.dump();
        }

        client.name = comm.params[0];
        client.surname = comm.params[1];
        client.patronymic = comm.params[2];
        client.pasport = comm.params[3];
        client.password = comm.params[4];
        client.id = _last_user_id++;

        nlohmann::json json_client = client; 
        std::string json_string = json_client.dump();
        
        _db->write(database::clients_info, client.pasport,
            json_string);
        _db->write(database::last_id, "last_user_id", std::to_string(_last_user_id));

        core::success_result_msg res("successful registration");
        nlohmann::json res_json = res;
        return res_json.dump();
    }

    std::string state::login(core::commands& comm, core::user& client)
    {
        core::error_msg err;
        if (!client.is_empty())
            err.error_msg = "already_authorized";
        if (comm.params.size() != 2)
            err.error_msg = "wrong_params"; 
        
        if (!err.error_msg.empty())
        {
            nlohmann::json err_json = err; 
            return err_json.dump();
        }
            
        client = get_user(comm.params[0]);

        if (client.is_empty())
            err.error_msg = "wrong_pasport";
        else if (client.password != comm.params[1])
            err.error_msg = "wrong_pass";

        if (!err.error_msg.empty())
        {
            nlohmann::json err_json = err; 
            return err_json.dump();
        }

        core::success_result_msg res("successful_logged");
        nlohmann::json res_json = res;
        return res_json.dump();
    }

    user state::get_user(std::string& pasport)
    {
        user client;
        std::string reply = _db->read(database::clients_info, pasport, "client not found");
        if (reply != "client not found") 
            client.from_json(nlohmann::json::parse(reply));
        return client;
    }

    bool state::client_exist(std::string& pasport)
    {
        if (_db->read(database::clients_info, pasport, "client not found") != "client not found") return true;
        return false;
    }

    void state::setup()
    {
        std::string res = _db->read(database::last_id, "last_user_id", "id not found");
        if (res != "id not found")
            _last_user_id = std::stoull(res);
        else 
            _db->write(database::last_id, "last_user_id", std::to_string(_last_user_id));
    }
}