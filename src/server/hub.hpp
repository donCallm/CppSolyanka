#pragma once
#include <boost/signals2.hpp>
#include <server/connect.hpp>
#include <memory>
#include "app.hpp"

#define STATE() (_application.get_state())

namespace core
{
    class app;
    class server;
    class msg;

    class hub : std::enable_shared_from_this<hub>
    {
    private:
        std::optional<user> get_user(const std::string& login);
        std::string get_balance(user& usr, uint64_t card_id);
        std::string get_cards(user& usr);
        std::string get_bank_accounts(user& usr);
    public:
        hub(app& application);
        virtual ~hub() = default;
        
        void start();

        void subscribe_on_connection(net::con_handler::ptr conn);
        void subscribe_on_server();

        void on_new_msg(net::con_handler::ptr conn, command comm);
        void on_new_connection(net::con_handler::ptr conn);

        virtual std::optional<msg> handle_create_user(command& comm);
        virtual std::optional<msg> handle_login(command& comm);
        virtual std::optional<msg> handle_create_bank_acc(command& comm);
        virtual std::optional<msg> handle_change_balance(command& comm);
        virtual std::optional<msg> handle_create_card(command& comm);

        virtual std::optional<msg> handle_get_bank_info(command& comm);
        virtual std::optional<msg> handler_get_info(command& comm);
    protected:
        app& _application;
        std::shared_ptr<server> _server;
    };
}