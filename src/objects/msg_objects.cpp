#include "msg_objects.hpp"

namespace core
{
    uint64_t msg::id = 0;

    msg::msg() {++id;}

    success_result_msg::success_result_msg() {}

    success_result_msg::success_result_msg(std::string data) : result_msg(data) {};

    void reply_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("reply_msg").get_to(reply_msg);
        json_data.at("id").get_to(id);
    }

    void error_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("error_msg").get_to(error_msg);
        json_data.at("id").get_to(id);
    }

    void success_result_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("result_msg").get_to(result_msg);
        json_data.at("id").get_to(id);
    }
}
