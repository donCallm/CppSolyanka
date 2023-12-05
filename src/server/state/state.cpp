#include "state.hpp"
#include "../utils.hpp"
#include <objects/finance.hpp>
#include <objects/msg_objects.hpp>
#include <objects/finance.hpp>
#include <spdlog/spdlog.h>
#include <server/database/database.hpp>
#include <filesystem>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace db;
using namespace boost::posix_time;

namespace core
{
    state::state() : 
        _last_user_id(0),
        _last_bank_acc_id(0),
        _last_card_id(0)
    {
        spdlog::info("Start state");
        setup();
    }

    uint64_t state::get_new_id(const identifier_type& type, const std::string key)
    { 
        std::lock_guard<std::mutex> _(_m);
        uint64_t tmp;

        switch (type)
        {
            case identifier_type::last_user_id: {
                tmp = _last_user_id.fetch_add(1);
                break; }
            case identifier_type::last_bank_acc_id: {
                tmp = _last_bank_acc_id.fetch_add(1);
                break; }
            case identifier_type::last_card_id: {
                tmp = _last_card_id.fetch_add(1);
                break; }
            case identifier_type::last_transaction_id: {
                tmp = _last_transaction_id.fetch_add(1);
                break; }
            default:
                break;
        }

        DB()->write(database::last_id, key, std::to_string(tmp));
        return tmp; 
    }

    bool state::create_user(user& usr)
    {
        if (get_user(usr.id).has_value() ||
            std::find(_active_users.begin(), _active_users.end(), usr.id) != _active_users.end())
            return false;

        usr.id = get_new_id(last_user_id, "last_user_id");
        _logins.insert(std::make_pair( usr.login, usr.id));

        nlohmann::json json_usr = usr;
        DB()->write(database::clients_info, std::to_string(usr.id), json_usr.dump());

        {
            std::string file_path(std::filesystem::current_path().generic_string() + "/state/logins.json");
            std::ofstream output_file(file_path);

            if (!output_file.is_open())
                return false;

            output_file << utils::to_str(_logins);
        }

        return true;
    }

    bool state::create_bank_account(user& usr)
    {
        if (usr.empty())
            return false;

        bank_account new_acc(get_new_id(last_bank_acc_id, "last_bank_acc_id"));
        nlohmann::json json_acc = new_acc;
        DB()->write(database::bank_accounts, std::to_string(new_acc.id), json_acc.dump());

        usr.bank_accounts.insert(new_acc.id);
        nlohmann::json json_usr = usr;
        DB()->write(database::clients_info, std::to_string(usr.id), json_usr.dump());

        return true;
    }

    bool state::login(uint64_t id, const std::string& password, user& client)
    {
        std::optional<user> res = get_user(id);
        if (res.has_value())
        {
            core::user usr = res.value();
            if (_active_users.find(usr.id) != _active_users.end())
                return false;
            
            if (usr.password == password)
            {
                client.id = usr.id;
                _active_users.insert(usr.id);
                return true;
            }
        }
        return false;
    }

    std::optional<int> state::get_balance(uint64_t card_id)
    {   
        std::optional<card> res_card = get_card(card_id);
        if(!res_card.has_value())
            return std::nullopt;

        std::optional<bank_account> res_acc = get_bank_account(res_card.value().bank_account_id);
        if (!res_acc.has_value())
            return std::nullopt;

        return res_acc.value().balance;
    }

    bool state::change_balance(command::type& operation, uint64_t sum, uint64_t card_id)
    {
        std::optional<card> res_card = get_card(card_id);
        if(!res_card.has_value())
            return false;

        std::optional<bank_account> res = get_bank_account(res_card.value().bank_account_id);
        if (!res.has_value())
            return false;
            
        bank_account acc = res.value();
        transaction ts(get_new_id(last_transaction_id, "last_transaction_id"), utils::to_str(second_clock::local_time()), sum);

        if (operation == command::replenish_balance)
        {
            ts.operation = transaction::replenishment;
            acc.balance += sum;
        }
        else if (operation == command::debit_funds)
        {
            if (sum > acc.balance)
                return false;

            ts.operation = transaction::debit;
            acc.balance -= sum;
        }
        else
        {
            return false;
        }

        acc.transactions_id.insert(ts.id);
        ts.completion_date = utils::to_str(second_clock::local_time());

        nlohmann::json json_transaction = ts;
        nlohmann::json json_acc = acc;
        DB()->write(database::bank_accounts, std::to_string(acc.id), json_acc.dump());
        DB()->write(database::transactions, std::to_string(ts.id), json_transaction.dump());

        return true;
    }

    void state::create_card(user& usr, uint64_t bank_account_id)
    {   
        card new_card(get_new_id(last_card_id, "last_card_id"), bank_account_id);
        usr.cards.insert(new_card.id);

        nlohmann::json json_usr = usr;
        nlohmann::json json_card = new_card;

        DB()->write(database::cards, std::to_string(new_card.id), json_card.dump());
        DB()->write(database::clients_info, std::to_string(usr.id), json_usr.dump());
    }

    std::optional<uint64_t> state::get_id(const std::string& login)
    {
        for (const auto& elem : _logins)
        {
            if (elem.first == login)
                return elem.second;
        }
        return std::nullopt;
    }

    std::optional<user> state::get_user(uint64_t id)
    {
        std::string res = DB()->read(database::clients_info, std::to_string(id));
        if (res.empty()) 
            return std::nullopt;

        user usr;
        usr.from_json(nlohmann::json::parse(res));
        return usr;
    }

    std::optional<bank_account> state::get_bank_account(uint64_t acc_id)
    {
        std::string res = DB()->read(database::bank_accounts, std::to_string(acc_id));

        if (res.empty())
            return std::nullopt;

        bank_account acc;
        acc.from_json(nlohmann::json::parse(res));
        return acc;
    }

    std::optional<card> state::get_card(uint64_t card_id)
    {
        std::string res = DB()->read(database::cards, std::to_string(card_id));
        if(res.empty())
            return std::nullopt;
        
        card crd;
        crd.from_json(nlohmann::json::parse(res));
        return crd;
    }

    void state::set_value(const std::string& value, const identifier_type& type)
    {
        if (value.empty())
        {
            switch (type)
            {
                case identifier_type::last_user_id: {
                    DB()->write(database::last_id, "last_user_id", std::to_string(_last_user_id));
                    break; }
                case identifier_type::last_bank_acc_id: {
                    DB()->write(database::last_id, "last_bank_acc_id", std::to_string(_last_bank_acc_id));
                    break; }
                case identifier_type::last_card_id: {
                    DB()->write(database::last_id, "last_card_id", std::to_string(_last_card_id));
                    break; }
                case identifier_type::last_transaction_id: {
                    DB()->write(database::last_id, "last_transaction_id", std::to_string(_last_transaction_id));
                    break; }
                default:
                    break;
            }
        }
        else
        {
            switch (type)
            {
                case identifier_type::last_user_id: {
                    _last_user_id = std::stoull(value);
                    break; }
                case identifier_type::last_bank_acc_id: {
                    _last_bank_acc_id = std::stoull(value);
                    break; }
                case identifier_type::last_card_id: {
                    _last_card_id = std::stoull(value);
                    break; }
                case identifier_type::last_transaction_id: {
                    _last_transaction_id = std::stoull(value);
                    break; }
                default:
                    break;
            }
        }
    }

    void state::set_logins()
    {
        std::string file_path(std::filesystem::current_path().generic_string() + "/state/logins.json");
        if (!std::filesystem::exists(file_path))
        {
            std::ofstream file(file_path);
            if (file.is_open())
            {
                spdlog::info("Successful read file");
                file << utils::to_str(_logins);
                file.close();
            }
            else
            {
                spdlog::error("Unable to open file for writing");
            }
        }
        else
        {
            std::ifstream file(file_path);

            if (file.is_open())
            {
                nlohmann::json json_data;
                file >> json_data;
                for (const auto& [key, value] : json_data.items()) {
                    std::string numeric_key = key;
                    _logins[numeric_key] = value;
                }
            }
            else
            {
                std::runtime_error("Unable to open file for reading");
            }
        }
    }

    void state::setup()
    {
        set_value(DB()->read(database::last_id, "last_user_id"), last_user_id);
        set_value(DB()->read(database::last_id, "last_bank_acc_id"), last_bank_acc_id);
        set_value(DB()->read(database::last_id, "last_card_id"), last_card_id);
        set_value(DB()->read(database::last_id, "last_transaction_id"), last_transaction_id);

        set_logins();
    }
}