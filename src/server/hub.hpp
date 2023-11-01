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

        bool validate(const command& comm, const uint64_t& number_of_params);

        msg handle_create_user(command& comm);
        msg handle_login(command& comm);

    private:
        app& _application;
        std::shared_ptr<server> _server;
    };

}