#include "state.hpp"

namespace server_state
{
    db::database* state::_db = db::database::get_instance();
    uint64_t state::_last_user_id = 1;
   
    std::string state::registration(core::commands& comm)
    {
        if (client_exist(comm.params[3])) return "already exist";
        if (!valid_number_of_params(6, comm)) return "wrong numbers of parametrs";
        if (comm.params.back() != "0") return "you cant registrate new acc while you are logged in";

        _db->write(db::database::db_list::clients_info, comm.params[3],
            comm.params[0] + " " + comm.params[1] + " " + comm.params[2] + " " + comm.params[4]);
        _db->write(db::database::db_list::last_id, "last_user_db", std::to_string(++_last_user_id));
        return std::to_string(_last_user_id);
    }

    std::string state::loggin(core::commands& comm)
    {
        if (!valid_number_of_params(3, comm)) return "wrong numbers of parametrs"; 
        if (comm.params.back() != "0") return "you already authorized";

        core::user us = get_user(comm.params[0]);
        
        if (us.is_empty()) return "wrong pasport or password";
        if (us.password != comm.params[4]) return "wrong_password";
        
        return std::to_string(us.id);
    }

    core::user state::get_user(std::string pasport)
    {
        core::user us;
        std::string reply = _db->read(db::database::db_list::clients_info, pasport);
        if (reply != "empty") us.set_data(reply);
        return us;
    }

    bool state::client_exist(std::string pasport)
    {
        if (_db->read(db::database::db_list::clients_info, pasport) != "empty") return true;
        return false;
    }

    bool state::valid_number_of_params(uint16_t correct_numb, core::commands& comm)
    {   
        if (comm.params.size() == correct_numb) return true;
        else return false;
    }

    void state::set_state()
    {
        _last_user_id = std::stoull(_db->read(db::database::db_list::last_id, "last_user_id"));
    }
}