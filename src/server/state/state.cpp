#include "state.hpp"

namespace server_state
{
    db::database* state::_db = db::database::get_instance();
    uint64_t state::_last_user_id = 1;
   
    std::string state::registration(core::commands& comm)
    {
        if (client_exist(std::stoull(comm.params[comm.params.size() - 1]))) return "already exist";
        if (!valid_number_of_params(5, comm)) return "wrong numbers of parametrs";
        if (comm.params[4] != "0") return "you cant registrate new acc while you are logged in";

        _db->write(db::database::db_list::clients_info, std::to_string(_last_user_id),
            comm.params[0] + " " + comm.params[1] + " " + comm.params[2] + " " + comm.params[3]);
        _db->write(db::database::db_list::last_id, "last_user_db", std::to_string(++_last_user_id));
        return std::to_string(_last_user_id);
    }

    

    core::user state::get_user(uint64_t user_id)
    {
        core::user us;
        std::string reply = _db->read(db::database::db_list::clients_info, std::to_string(user_id));
        if (reply != "empty") us.set_data(reply);
        return us;
    }

    bool state::client_exist(uint64_t user_id)
    {
        if (_db->read(db::database::db_list::clients_info, std::to_string(user_id)) != "empty") return true;
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