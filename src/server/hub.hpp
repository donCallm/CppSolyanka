#pragma once
#include <boost/signals2.hpp>
#include <server/connect.hpp>
#include <memory>

namespace core
{
    class app;
    class server;
    class reply_msg;

    class hub : std::enable_shared_from_this<hub>
    {
    public:
        hub(app& application);
        
        void subscribe_on_connection(net::con_handler::ptr conn);

        void on_new_msg(net::con_handler::ptr conn, command comm);
        void on_new_connection(net::con_handler::ptr conn);
        
        void lack_of_token();
        void subscribe_on_server();

        reply_msg handle_create_user(command& comm);
        reply_msg handle_login(command& comm);

    private:
        app& _application;
        std::shared_ptr<server> _server;
    };

}