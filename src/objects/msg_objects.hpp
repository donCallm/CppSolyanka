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
        operator nlohmann::json() const { return nlohmann::json{ {"rpl_msg", rpl_msg}, {"id", id} }; }
        reply_msg() {}
        reply_msg(std::string data);

    public:
        void from_json(const nlohmann::json& json_data);

    public:
        std::string rpl_msg;
    };

    class error_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"err_msg", err_msg}, {"id", id} }; };
        error_msg() {}
        error_msg(std::string data);
        
    public:
        void from_json(const nlohmann::json& json_data);

    public:
        std::string err_msg;
    };

    class success_result_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"res_msg", res_msg}, {"id", id} }; }
        success_result_msg() {}
        success_result_msg(std::string data);

    public:
        void from_json(const nlohmann::json& json_data);

    public:
        std::string res_msg;
    };
}