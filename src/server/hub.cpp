#include "hub.hpp"
#include "app.hpp"
#include "server.hpp"
#include "utils.hpp"
#include <objects/commands.hpp>
#include <objects/msg_objects.hpp>
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>
using namespace utils;
namespace core
{

hub::hub(app& application) :
    _application(application),
    _server(std::make_shared<server>(_application.get_service()))
{
    spdlog::info("Start hub");
    subscribe_on_server();
}

void hub::subscribe_on_server()
{
    _server->on_accept_connection.connect(boost::bind(&hub::on_new_connection, this, _1));
}

void hub::on_new_connection(net::con_handler::ptr conn)
{
    subscribe_on_connection(conn);
}

void hub::subscribe_on_connection(net::con_handler::ptr conn)
{
    conn->on_msg.connect(boost::bind(&hub::on_new_msg, this, _1, _2));
}

void hub::on_new_msg(net::con_handler::ptr conn, command comm)
{
    msg rpl;
    nlohmann::json json_data;

    switch (comm.instruction)
    {
        case command::registration: {
            rpl = handle_create_user(comm);
            break; }
        case command::login: {
            rpl = handle_login(comm);
            break; }
        case command::ping: {
            rpl.message = to_str<success_result_msg>("pong");
            spdlog::info("server sending pong");
            break; }
        case command::end: {
            spdlog::info("user {} disconnect", conn->get_adress());
            conn->drop();
            break; }
        default:
        {
            rpl.message = to_str<success_result_msg>("unknown_command");
            spdlog::warn("get unknown command");
            break; 
        }
    }
    json_data = rpl; 
    std::string json_string = json_data.dump();
    conn->send(json_string);
}

bool hub::validate_params(const command& comm,  const uint64_t& number_of_params)
{
    if (number_of_params != comm.params.size())
        return false;
    return true;
}

msg hub::handle_login(command& comm)
{
    if (!validate_params(comm, 2))
    {
        msg rpl(to_str<error_msg>("Wrong  params"));
        return rpl;
    }

    if (!_application.get_state()->login(comm.params[0], comm.params[1]))
    {
        msg rpl(to_str<error_msg>("Error of login"));
        return rpl;
    }

    msg rpl(to_str<error_msg>("Successful login"));
    return rpl;
}


msg hub::handle_create_user(command& comm)
{
    if (!validate_params(comm, 5))
    {
        msg rpl(to_str<error_msg>("Wrong  params"));
        return rpl;
    }

    core::user usr(comm.params[0], comm.params[1], 
        comm.params[2], comm.params[3], comm.params[4]);

    if (_application.get_state()->create_user(usr))
    {
        msg rpl(to_str<error_msg>("Error of registration"));
        return rpl;
    }
    
    msg rpl(to_str<success_result_msg>("Successful registration"));
    return rpl;
}

}