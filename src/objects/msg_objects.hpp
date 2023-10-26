#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class msg
    {
    public:
        msg();

    public:
        static uint64_t id;
    };

    class reply_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"reply_msg", reply_msg}, {"id", id} }; }

    public:
        void from_json(const nlohmann::json& json_data);

    public:
        std::string reply_msg;
    };

    class error_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"error_msg", error_msg}, {"id", id} }; };
        
    public:
        void from_json(const nlohmann::json& json_data);

    public:
        std::string error_msg;
    };

    class success_result_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"result_msg", result_msg}, {"id", id} }; }
        success_result_msg();
        success_result_msg(std::string data);

    public:
        void from_json(const nlohmann::json& json_data);

    public:
        std::string result_msg;
    };
}