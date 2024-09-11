#include "hub.hpp"
#include "app.hpp"
#include "server.hpp"
#include <objects/commands.hpp>
#include <objects/msg_objects.hpp>
#include <spdlog/spdlog.h>
#include <boost/bind.hpp>

namespace core
{
    hub::hub(app &application) : _application(application) {}

    void hub::start()
    {
        spdlog::info("Start hub");
        _server = std::make_shared<core::server>(_application.get_service(), _application.get_port());
        _server->start();
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

    void hub::on_new_msg(net::con_handler::ptr conn, std::string msg_data)
    {
        msg rpl;
        command::type t = to_command(msg_data);
        switch (t)
        {
            case command::ping:
            {
                rpl.message ="pong";
                spdlog::info("Server sending pong");
                break;
            }
            case command::end:
            {
                spdlog::info("Client {} disconnect", conn->get_adress());
                conn->drop();
                return;
            }
            case command::auth:
            {
                spdlog::info("Get auth message");
                return;
            }
            default:
            {
                rpl.message ="unknown command";
                spdlog::warn("Get unknown command");
                break;
            }
        }
        conn->send(rpl.message);
    }


}