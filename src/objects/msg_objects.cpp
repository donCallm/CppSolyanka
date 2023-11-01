#include <objects/msg_objects.hpp>

namespace core
{
    uint64_t msg::id = 0;

    msg::msg() {++id;}

    success_result_msg::success_result_msg(std::string data) {message = data;}

    error_msg::error_msg(std::string data) {message = data;}

    msg::msg(std::string data) : message(data) {}

    void msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("message").get_to(message);
        json_data.at("id").get_to(id);
    }

    void error_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("err_msg").get_to(message);
        json_data.at("id").get_to(id);
    }

    void success_result_msg::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("res_msg").get_to(message);
        json_data.at("id").get_to(id);
    }
}