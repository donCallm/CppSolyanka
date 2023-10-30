#include "hub.hpp"
#include "app.hpp"
#include "server.hpp"
#include <objects/commands.hpp>
#include <objects/msg_objects.hpp>
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>

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
    success_result_msg res;
    reply_msg rpl;
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
            res.res_msg = "pong";
            json_data = res;
            rpl.rpl_msg = json_data.dump();
            spdlog::info("server sending pong");
            break; }
        case command::end: {
            spdlog::info("user {} disconnect", conn->get_adress());
            conn->drop();
            break; }
        default:
        {
            res.res_msg = "unknown_command";
            json_data = res;
            rpl.rpl_msg = json_data.dump();
            spdlog::warn("get unknown command");
            break; 
        }
    }
    json_data = rpl; 
    std::string json_string = json_data.dump();
    conn->send(json_string);
}

reply_msg hub::handle_login(command& comm)
{
    if (comm.params.size() != 5)
    {
        core::error_msg err("wrong params");
        nlohmann::json json_error = err;
        reply_msg rpl(json_error.dump());
        return rpl;
    }

    if (!_application.get_state()->login(comm.params[0], comm.params[1]))
    {
        core::error_msg err("error of login");
        nlohmann::json json_error = err;
        reply_msg rpl(json_error.dump());
        return rpl;
    }

    success_result_msg res("Successful login");
    nlohmann::json json_result = res;
    reply_msg rpl(json_result.dump());

    return rpl;
}

reply_msg hub::handle_create_user(command& comm)
{

    if (comm.params.size() != 5)
    {
        core::error_msg err("wrong params");
        nlohmann::json json_error = err;
        reply_msg rpl(json_error.dump());
        return rpl;
    }

    core::user usr(comm.params[0], comm.params[1], 
        comm.params[2], comm.params[3], comm.params[4]);

    if (_application.get_state()->create_user(usr))
    {
        core::error_msg err("error of registration");
        nlohmann::json json_error = err;
        reply_msg rpl(json_error.dump());
        return rpl;
    }

    success_result_msg res("Successful registration");
    nlohmann::json json_result = res;
    reply_msg rpl(json_result.dump());

    return rpl;
}

}