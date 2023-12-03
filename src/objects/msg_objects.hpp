#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"message", message}, {"id", id}, {"params", params} }; };
        explicit msg(std::string data);
        msg();
        ~msg(){}
    
    public:
        void msg_from_json(const nlohmann::json& json_data);
        void set_message(const std::string& data);

    public:
        static uint64_t id;
        std::string message;
        std::vector<std::string> params;
    };

    class error_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"err_msg", message}, {"id", id}, {"params", params} }; };
        error_msg() {}
        explicit error_msg(std::string data);
        ~error_msg(){}
        
    public:
        void error_from_json(const nlohmann::json& json_data);
    };

    class success_result_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"res_msg", message}, {"id", id}, {"params", params}}; }
        success_result_msg() {}
        explicit success_result_msg(std::string data);
        ~success_result_msg(){}

    public:
        void success_result_from_json(const nlohmann::json& json_data);
    };
}