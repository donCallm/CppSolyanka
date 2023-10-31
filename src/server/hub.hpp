#pragma once
#include <boost/signals2.hpp>
#include <server/connect.hpp>
#include <memory>

namespace core
{
    class app;
    class server;
    class msg;

    class hub : std::enable_shared_from_this<hub>
    {
    public:
        hub(app& application);
        
        void subscribe_on_connection(net::con_handler::ptr conn);

        void on_new_msg(net::con_handler::ptr conn, command comm);
        void on_new_connection(net::con_handler::ptr conn);
        
        void subscribe_on_server();

        std::string get_error(const std::string& err_msg);
        std::string get_result(const std::string& res_msg);

        msg handle_create_user(command& comm);
        msg handle_login(command& comm);

    private:
        app& _application;
        std::shared_ptr<server> _server;
    };

}