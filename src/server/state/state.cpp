#include "state.hpp"
using db::database;

namespace server
{
    state::state(): _last_user_id(0), _db(db::database::get_instance()) {}

    reply state::registration(core::commands& comm, core::user& client)
    {
        reply rpl;

        if (!client.is_empty()) 
            rpl.err =  reply::already_authorized;
        if (comm.params.size() != 5)
            rpl.err = reply::wrong_params;
        if (client_exist(comm.params[3]))
            rpl.err = reply::already_exist;

        auto err = reply::error_messages.find(rpl.err);
        if (err != reply::error_messages.end())
            return rpl;

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
        rpl.msg = reply::successful_registration;
        return rpl;
    }

    reply state::login(core::commands& comm, core::user& client)
    {
        reply rpl;
        if (!client.is_empty())
            rpl.err = reply::already_authorized;
        if (comm.params.size() != 2)
            rpl.err = reply::wrong_params; 
        
        auto err = reply::error_messages.find(rpl.err);
        if (err != reply::error_messages.end())
            return rpl;
            
        client = get_user(comm.params[0]);

        if (client.is_empty())
            rpl.err = reply::wrong_pasport;
        else if (client.password != comm.params[1])
            rpl.err = reply::wrong_pass;
        else
            rpl.msg = reply::successful_logged;

        return rpl;
    }

    user state::get_user(std::string& pasport)
    {
        user client;
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