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
    hub::hub(app &application) : _application(application) {}

    void hub::start()
    {
        spdlog::info("Start hub");
        _server = std::make_shared<core::server>(_application.get_service());
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

    void hub::on_new_msg(net::con_handler::ptr conn, command comm)
    {
        msg rpl;
        nlohmann::json json_data;

        switch (comm.instruction)
        {
            case command::registration:
            {
                rpl = handle_create_user(comm).value();
                break;
            }
            case command::login:
            {
                rpl = handle_login(comm).value();
                break;
            }
            case command::create_bank_acc:
            {
                rpl = handle_create_bank_acc(comm).value();
                break;
            }
            case command::get_balance:
            case command::get_cards:
            case command::get_bank_accounts:
            {
                rpl = handle_get_bank_info(comm).value();
                break;
            }
            case command::replenish_balance:
            case command::debit_funds:
            {
                rpl = handle_change_balance(comm).value();
                break;
            }
            case command::create_card:
            {
                rpl = handle_create_card(comm).value();
                break;
            }
            case command::get_info:
            {
                rpl = handler_get_info(comm).value();
                break;
            }
            case command::ping:
            {
                rpl.message = to_str<msg>("pong");
                spdlog::info("server sending pong");
                break;
            }
            case command::end:
            {
                spdlog::info("user {} disconnect", conn->get_adress());
                conn->drop();
                break;
            }
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

    std::optional<user> hub::get_user(const std::string& login)
    {
        auto id = STATE()->get_id(login);
        if (!id.has_value())
            return std::nullopt;
        
        auto usr = STATE()->get_user(id.value());
        if (!usr.has_value())
            return std::nullopt;
        
        return usr.value();
    }

    std::string hub::get_balance(user& usr, uint64_t card_id)
    {
        auto iter = usr.cards.find(card_id);
        if (iter == usr.cards.end())
            return "Error of geting balance";
        return "Your balance: " + std::to_string(STATE()->get_balance(*iter).value());
    }

    std::string hub::get_cards(user& usr)
    {
        if (usr.cards.size() == 0)
            return "No active cards";
        
        std::string res = "Id ur cards: ";
        for (const auto &card : usr.cards)
            res += std::to_string(card) + " ";
        return res;
    }

    std::string hub::get_bank_accounts(user& usr)
    {
        if (usr.bank_accounts.size() == 0)
            return "No active bank accounts";
        
        std::string res = "Id ur bank accounts: ";
        for (const auto &acc : usr.bank_accounts)
            res += std::to_string(acc) + " ";
        return res;
    }

    std::optional<msg> hub::handle_login(command &comm)
    {
        msg rpl;

        if (!validate_params(comm.params, 3))
        {
            rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
            return rpl;
        }
        
        if (!validate_params(comm.params, RGX_LTRS_NUMS))
        {
            rpl.set_message(to_str<error_msg>("Invalid symmbol. Only letters and numbers can be used"));
            return rpl;
        }

        auto id = STATE()->get_id(comm.params[0]);
        if (!id.has_value())
        {
            rpl.set_message(to_str<error_msg>("Error of login"));
            return rpl;
        }

        auto res = STATE()->login(id.value(), comm.params[1]);
        if (res.has_value())
        {
            rpl.set_message(to_str<error_msg>(res.value()));
        }
        else
        {
            rpl.set_message((to_str<success_result_msg>("Successful login")));
            rpl.params.push_back(std::to_string(id.value()));
        }

        return rpl;
    }

    std::optional<msg> hub::handle_create_user(command &comm)
    {
        msg rpl;

        if (!validate_params(comm.params, 7))
        {
            rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
            return rpl;
        }

        if (!validate_params(comm.params, RGX_LTRS_NUMS))
        {
            rpl.set_message(to_str<error_msg>("Invalid symmbol. Only letters and numbers can be used"));
            return rpl;
        }
        
        if (!validate_params(std::vector<std::string>{ comm.params[1], comm.params[2], comm.params[3] }, "^[a-zA-Z]+$"))
        {
            rpl.set_message(to_str<error_msg>("Invalid symmbol. Last name, first name and patronymic can only have letters"));
            return rpl;
        }

        user usr(comm.params[0], comm.params[1], comm.params[2], comm.params[3], comm.params[4], comm.params[5]);
        auto res = STATE()->create_user(usr);
        if (res.has_value())
            rpl.set_message(to_str<error_msg>(res.value()));
        else
            rpl.set_message((to_str<success_result_msg>("Successful registration")));

        return rpl;
    }

    std::optional<msg> hub::handle_create_bank_acc(command &comm)
    {
        msg rpl;

        if (!validate_params(comm.params, 1))
        {
            rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
            return rpl;
        }

        if (!is_number(comm.params[0]))
        {
            rpl.set_message(to_str<error_msg>("Error. ID would be number"));
            return rpl;
        }
        
        try
        {
            auto usr = STATE()->get_user(std::stoull(comm.params[0]));
            if (!usr.has_value())
            {
                rpl.set_message(to_str<error_msg>("Error. User not found"));
                return rpl;
            }

            if (usr.value().cards.size() == MAX_BANK_ACCS_COUNT)
            {
                rpl.set_message(to_str<error_msg>("You have reached your bank acc limit"));
                return rpl;
            }
            
            if (!STATE()->create_bank_account(usr.value()))
                rpl.set_message(to_str<error_msg>("Error of create new bank account"));
            else
                rpl.set_message((to_str<success_result_msg>("Successful create new bank account")));
        }
        catch (const std::exception &e)
        {
            spdlog::error("Error of create new bank account: {}", e.what());
            rpl.set_message(to_str<error_msg>("Error of create new bank account"));
            return rpl;
        }
        return rpl;
    }

    std::optional<msg> hub::handle_get_bank_info(command &comm)
    {
        msg rpl;

        switch (comm.instruction)
        {
            case command::get_balance:
            {
                if (!validate_params(comm.params, 2))
                {
                    rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
                    return rpl;
                }
                break;
            }
            case command::get_cards:
            case command::get_bank_accounts:
            {
                if (!validate_params(comm.params, 1))
                {
                    rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
                    return rpl;
                }
                break;
            }
            default:
                break;
        }
        
        if (!validate_params(comm.params, RGX_NUMS) )
        {
            rpl.set_message(to_str<error_msg>("Invalid symmbol. Only numbers can be used"));
            return rpl;
        }

        if (!is_number(comm.params[comm.params.size() - 1]))
        {
            rpl.set_message(to_str<error_msg>("Error change balance"));
            return rpl;
        }

        try
        {
            auto usr = STATE()->get_user(std::stoull(comm.params[comm.params.size() - 1]));
            if (!usr.has_value())
            {
                rpl.set_message(to_str<error_msg>("User not found"));
                return rpl;
            }
            std::string res;

            switch (comm.instruction)
            {
                case command::get_balance:
                {
                    res = get_balance(usr.value(), std::stoull(comm.params[0]));
                    break;
                }
                case command::get_cards:
                {
                    res = get_cards(usr.value());
                    break;
                }
                case command::get_bank_accounts:
                {
                    res = get_bank_accounts(usr.value());
                    break;
                }
                default:
                {
                    res = "Error of geting info";
                    break;
                }
            }

            rpl.set_message((to_str<success_result_msg>(res)));
        }
        catch (const std::exception &e)
        {
            spdlog::error("Error of geting info: {}", e.what());
            rpl.set_message(to_str<error_msg>("Error of geting info"));
            return rpl;
        }

        return rpl;
    }

    std::optional<msg> hub::handle_change_balance(command &comm)
    {
        msg rpl;

        if (!validate_params(comm.params, 3))
        {
            rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
            return rpl;
        }

        if (!validate_params(comm.params, RGX_NUMS))
        {
            rpl.set_message(to_str<error_msg>("Invalid symmbol. Only numbers can be used"));
            return rpl;
        }

        auto usr = STATE()->get_user(std::stoull(comm.params[2]));
        if (!usr.has_value())
        {
            rpl.set_message(to_str<error_msg>("User not found"));
            return rpl;
        }

        std::unique_ptr<tx_send> transaction = std::make_unique<tx_send>(STATE()->get_new_id(state::last_tx_id, LAST_TX_ID),
            std::stoull(comm.params[1]), usr.value().id, std::stoull(comm.params[0]));

        if (comm.instruction == command::replenish_balance)
            transaction->snd_type = tx_send::replenish_balance;
        else
            transaction->snd_type = tx_send::debit_funds;

        _application.get_binder()->validate(std::move(transaction));
        rpl.set_message((to_str<success_result_msg>("Transaction accepted for processing")));
        
        return rpl;
    }

    std::optional<msg> hub::handle_create_card(command &comm)
    {
        msg rpl;

        if (!validate_params(comm.params, 2))
        {
            rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
            return rpl;
        }

        if (!validate_params(comm.params, RGX_NUMS))
        {
            rpl.set_message(to_str<error_msg>("Invalid symmbol. Only numbers can be used"));
            return rpl;
        }

        auto usr = STATE()->get_user(std::stoull(comm.params[1]));
        if (!usr.has_value())
        {
            rpl.set_message(to_str<error_msg>("User not found"));
            return rpl;
        }
    
        if (usr.value().cards.size() == MAX_CARDS_COUNT)
        {
            rpl.set_message(to_str<error_msg>("You have reached your card limit"));
            return rpl;
        }

        STATE()->create_card(usr.value(), std::stoull(comm.params[0]));
        rpl.set_message((to_str<success_result_msg>("Successful created")));

        return rpl;
    }

    std::optional<msg> hub::handler_get_info(command& comm)
    {
        msg rpl;

        if (!validate_params(comm.params, 1))
        {
            rpl.set_message(to_str<error_msg>("Wrong numbers of parametrs"));
            return rpl;
        }

        if (comm.params[0] == "0") 
        {
            rpl.set_message(to_str<error_msg>("You need to be login"));
            return rpl;
        }

        auto usr = STATE()->get_user(std::stoull(comm.params[0]));
        if (!usr.has_value())
        {
            rpl.set_message(to_str<error_msg>("User not found"));
            return rpl;
        }

        nlohmann::json json_usr = usr.value();
        rpl.set_message(to_str<success_result_msg>(json_usr.dump()));
        return rpl;
    }
}