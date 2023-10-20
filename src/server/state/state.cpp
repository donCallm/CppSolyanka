#include "state.hpp"
using db::database;

namespace server
{
    state::operation_result state::registration(core::commands& comm, core::user& client)
    {
        if (!client.is_empty())
            return already_authorized;
        if (comm.params.size() != 5)
            return wrong_params;
        if (client_exist(comm.params[3]))
            return already_exist;

        client.name = comm.params[0];
        client.surname = comm.params[1];
        client.patronymic = comm.params[2];
        client.pasport = comm.params[3];
        client.password = comm.params[4];
        client.id = _last_user_id++;

        nlohmann::json serialize_message = client; 
        std::string json_string = serialize_message.dump();
        
        _db->write(database::clients_info, client.pasport,
            json_string);
        _db->write(database::last_id, "last_user_id", std::to_string(_last_user_id));
        return successful_registration;
    }

    state::operation_result state::login(core::commands& comm, core::user& client)
    {
        if (!client.is_empty())
            return already_authorized;
        if (comm.params.size() != 2)
            return wrong_params; 
            
        client = get_user(comm.params[0]);

        if (client.is_empty())
            return wrong_pasport;
        if (client.password != comm.params[1])
            return wrong_pass;
        
        return successful_logged;
    }

    core::user state::get_user(std::string& pasport)
    {
        core::user client;
        std::string reply = _db->read(database::clients_info, pasport);
        if (reply != "empty") 
            client.from_json(nlohmann::json::parse(reply));
        return client;
    }

    bool state::client_exist(std::string& pasport)
    {
        if (_db->read(database::clients_info, pasport) != "empty") return true;
        return false;
    }

    void state::initialize()
    {
        try
        {
            _last_user_id = std::stoull(_db->read(database::last_id, "last_user_id"));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
}