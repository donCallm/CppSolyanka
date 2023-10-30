#include <objects/msg_objects.hpp>

namespace core
{
    uint64_t msg::id = 0;

    msg::msg() {++id;}

    success_result_msg::success_result_msg(std::string data) : res_msg(data) {};
    
    error_msg::error_msg(std::string data) : err_msg(data) {}

    reply_msg::reply_msg(std::string data) : rpl_msg(data) {}

    void reply_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("rpl_msg").get_to(rpl_msg);
        json_data.at("id").get_to(id);
    }

    void error_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("err_msg").get_to(err_msg);
        json_data.at("id").get_to(id);
    }

    void success_result_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("res_msg").get_to(res_msg);
        json_data.at("id").get_to(id);
    }
}