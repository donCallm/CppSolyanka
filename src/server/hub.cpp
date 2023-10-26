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

void hub::subscribe_on_connection(net::con_handler::ptr conn)
{
    conn->on_msg.connect(boost::bind(&hub::on_new_msg, this, _1, _2));
}

void hub::on_new_connection(net::con_handler::ptr conn)
{
    subscribe_on_connection(conn);
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
            res.result_msg = "pong";
            json_data = res;
            rpl.reply_msg = json_data.dump();
            spdlog::info("server sending pong");
            break; }
        case command::end: {
            spdlog::info("user {} disconnect", conn->get_adress());
            conn->drop();
            break; }
        default:
        {
            res.result_msg = "unknown_command";
            json_data = res;
            rpl.reply_msg = json_data.dump();
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
    reply_msg rpl;
    return rpl;
}

reply_msg hub::handle_create_user(command& comm)
{
    reply_msg rpl;
    return rpl;
}

}