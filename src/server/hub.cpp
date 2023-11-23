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
            rpl = handle_create_user(comm).value();
            break; }
        case command::login: {
            rpl = handle_login(comm).value();
            break; }
        case command::create_bank_acc: {
            rpl = handle_create_bank_acc(comm).value();
            break; }
        case command::ping: {
            rpl.message = to_str<msg>("pong");
            spdlog::info("server sending pong");
            break; }
        case command::end: {
            spdlog::info("user {} disconnect", conn->get_adress());
            conn->drop();
            break; }
        default:
        {
            rpl.message = to_str<msg>("unknown_command");
            spdlog::warn("get unknown command");
            break; 
        }
    }
    json_data = rpl; 
    std::string json_string = json_data.dump();
    conn->send(json_string);
}

std::optional<std::string> hub::validate_params(const command& comm,  const uint64_t& number_of_params)
{
    if (number_of_params != comm.params.size())
        return std::string(to_str<error_msg>("Invalid number of parameters"));
    return {};
}

std::optional<msg> hub::handle_login(command& comm)
{
    std::optional<std::string> params_valid = validate_params(comm, 3);
    msg rpl;

    if (params_valid.has_value())
    {
        rpl.set_message(to_str<error_msg>(params_valid.value()));
        return rpl;
    }

    user usr;
    if (!_application.get_state()->login(comm.params[0], comm.params[1], usr))
    {
        rpl.set_message(to_str<error_msg>("Error of login"));
    }
    else
    {
        rpl.set_message((to_str<success_result_msg>("Successful login")));
        rpl.params.push_back(std::to_string(usr.id));
    }
    
    return rpl;
}

std::optional<msg> hub::handle_create_user(command& comm)
{
    std::optional<std::string> res = validate_params(comm, 7);
    msg rpl;

    if (res.has_value())
    {
        rpl.set_message(to_str<error_msg>(res.value()));
        return rpl;
    }

    user usr(comm.params[0], comm.params[1], comm.params[2], comm.params[3], comm.params[4], comm.params[5]);
    if (!_application.get_state()->create_user(usr))
        rpl.set_message(to_str<error_msg>("Error of registraton"));
    else
        rpl.set_message((to_str<success_result_msg>("Successful registration")));

    return rpl;
}

std::optional<msg> hub::handle_create_bank_acc(command& comm)
{
    std::optional<std::string> res = validate_params(comm, 1);
    msg rpl;

    if (res.has_value())
    {
        rpl.set_message(to_str<error_msg>(res.value()));
        return rpl;
    }

    try
    {
        std::optional<std::string> login = _application.get_state()->get_login(std::stoull(comm.params[0])).value();
        std::optional<user> usr = _application.get_state()->get_user(login.value());
        
        if (!_application.get_state()->create_bank_account(usr.value()))
            rpl.set_message(to_str<error_msg>("Error of create new bank account"));
        else
            rpl.set_message((to_str<success_result_msg>("Successful create new bank account")));
    }
    catch(const std::exception& e)
    {
        spdlog::err(e.what());
        rpl.set_message(to_str<error_msg>("Error of create new bank account"));
    }

    return rpl;
}

}