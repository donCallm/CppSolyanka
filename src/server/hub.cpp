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

    hub::hub(app &application) : _application(application),
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

    std::optional<std::string> hub::validate_params(const command &comm, const uint64_t &number_of_params)
    {
        if (number_of_params != comm.params.size())
            return std::string(to_str<error_msg>("Invalid number of parameters"));
        return {};
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

    std::optional<msg> hub::handle_login(command &comm)
    {
        std::optional<std::string> params_valid = validate_params(comm, 3);
        msg rpl;

        if (params_valid.has_value())
        {
            rpl.set_message(to_str<error_msg>(params_valid.value()));
            return rpl;
        }

        user usr;
        auto id = STATE()->get_id(comm.params[0]);

        if (!id.has_value())
        {
            rpl.set_message(to_str<error_msg>("Error of login"));
            return rpl;
        }

        if (!STATE()->login(id.value(), comm.params[1], usr))
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

    std::optional<msg> hub::handle_create_user(command &comm)
    {
        std::optional<std::string> res = validate_params(comm, 7);
        msg rpl;

        if (res.has_value())
        {
            rpl.set_message(to_str<error_msg>(res.value()));
            return rpl;
        }

        user usr(comm.params[0], comm.params[1], comm.params[2], comm.params[3], comm.params[4], comm.params[5]);
        if (!STATE()->create_user(usr))
            rpl.set_message(to_str<error_msg>("Error of registraton"));
        else
            rpl.set_message((to_str<success_result_msg>("Successful registration")));

        return rpl;
    }

    std::optional<msg> hub::handle_create_bank_acc(command &comm)
    {
        std::optional<std::string> res = validate_params(comm, 1);
        msg rpl;

        if (res.has_value())
        {
            rpl.set_message(to_str<error_msg>(res.value()));
            return rpl;
        }

        if (!is_number(comm.params[0]))
        {
            rpl.set_message(to_str<error_msg>("Error of create new bank account"));
            return rpl;
        }
        
        try
        {
            auto usr = STATE()->get_user(std::stoull(comm.params[0]));
            if (!usr.has_value())
            {
                rpl.set_message(to_str<error_msg>("Error of create new bank account"));
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
        std::optional<std::string> res;
        msg rpl;

        switch (comm.instruction)
        {
            case command::get_balance:
            {
                res = validate_params(comm, 2);
                break;
            }
            case command::get_cards:
            case command::get_bank_accounts:
            {
                res = validate_params(comm, 1);
                break;
            }
            default:
                break;
        }

        if (res.has_value())
        {
            rpl.set_message(to_str<error_msg>(res.value()));
            return rpl;
        }
        spdlog::info(comm.params[comm.params.size() - 1]);
        if (!is_number(comm.params[comm.params.size() - 1]))
        {
            rpl.set_message(to_str<error_msg>("Error of geting info"));
            return rpl;
        }

        try
        {
            auto usr = STATE()->get_user(std::stoull(comm.params[comm.params.size() - 1]));
            if (!usr.has_value())
            {
                rpl.set_message(to_str<error_msg>("Error of geting info"));
                return rpl;
            }
            std::string res;

            switch (comm.instruction)
            {
                case command::get_balance:
                {
                    auto iter = usr.value().cards.find(std::stoull(comm.params[0]));
                    if (iter == usr.value().cards.end())
                    {
                        res = "Error of geting balance";
                    }
                    else
                    {
                        res = std::to_string(STATE()->get_balance(*iter).value());
                        rpl.set_message(utils::to_str<success_result_msg>(res));
                    }
                    break;
                }
                case command::get_cards:
                {
                    res = "Id ur cards: ";
                    for (const auto &card : usr.value().cards)
                        res += std::to_string(card) + " ";
                    break;
                }
                case command::get_bank_accounts:
                {
                    res = "Id ur bank accounts: ";
                    for (const auto &acc : usr.value().bank_accounts)
                        res += std::to_string(acc) + " ";
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
        std::optional<std::string> res = validate_params(comm, 3);
        msg rpl;

        if (res.has_value())
        {
            rpl.set_message(to_str<error_msg>(res.value()));
            return rpl;
        }

        if (!is_number(comm.params[2]))
        {
            rpl.set_message(to_str<error_msg>("Error change balance"));
            return rpl;
        }

        auto usr = STATE()->get_user(std::stoull(comm.params[2]));
        if (!usr.has_value())
        {
            rpl.set_message(to_str<error_msg>("Error change balance"));
            return rpl;
        }

        auto iter = usr.value().cards.find(std::stoull(comm.params[0]));
        if (STATE()->change_balance(comm.instruction, std::stoull(comm.params[1]), *iter))
            rpl.set_message(to_str<success_result_msg>("Successful change balance"));
        else
            rpl.set_message(to_str<error_msg>("Error change balance"));

        return rpl;
    }

    std::optional<msg> hub::handle_create_card(command &comm)
    {
        std::optional<std::string> res = validate_params(comm, 2);
        msg rpl;

        if (res.has_value())
        {
            rpl.set_message(to_str<error_msg>(res.value()));
            return rpl;
        }

        if (!is_number(comm.params[2]))
        {
            rpl.set_message(to_str<error_msg>("Error created card"));
            return rpl;
        }

        auto usr = STATE()->get_user(std::stoull(comm.params[1]));
        if (!usr.has_value())
        {
            rpl.set_message(to_str<error_msg>("Error of created"));
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
}