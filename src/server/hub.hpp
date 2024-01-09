#pragma once
#include <boost/signals2.hpp>
#include <server/connect.hpp>
#include <memory>

#define STATE() (_application.get_state())

namespace core
{
    class app;
    class server;
    class msg;

    class hub : std::enable_shared_from_this<hub>
    {
    private:
        bool validate_params(const std::vector<std::string>& params, const uint64_t &number_of_params);
        bool validate_params(const std::vector<std::string>& params, const std::string& pattern);
        std::optional<user> get_user(const std::string& login);
        std::string get_balance(user& usr, uint64_t card_id);
        std::string get_cards(user& usr);
        std::string get_bank_accounts(user& usr);
    public:
        hub(app& application);
        
        void subscribe_on_connection(net::con_handler::ptr conn);

        void on_new_msg(net::con_handler::ptr conn, command comm);
        void on_new_connection(net::con_handler::ptr conn);
        
        void subscribe_on_server();

        std::optional<msg> handle_create_user(command& comm);
        std::optional<msg> handle_login(command& comm);
        std::optional<msg> handle_create_bank_acc(command& comm);
        std::optional<msg> handle_change_balance(command& comm);
        std::optional<msg> handle_create_card(command& comm);

        std::optional<msg> handle_get_bank_info(command& comm);
        std::optional<msg> handler_get_info(command& comm);

        std::optional<msg> handler_clear_dbs(command& comm);

    private:
        app& _application;
        std::shared_ptr<server> _server;
    };

}