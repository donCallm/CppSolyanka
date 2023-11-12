#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"message", message}, {"id", id} }; };
        msg(std::string data);
        msg();
    
    public:
        void from_json(const nlohmann::json& json_data);
        void set_message(const std::string& data);

    public:
        static uint64_t id;
        std::string message;
    };

    class error_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"err_msg", message}, {"id", id} }; };
        error_msg() {}
        error_msg(std::string data);
        
    public:
        void from_json(const nlohmann::json& json_data);
    };

    class success_result_msg : public msg
    {
    public:
        operator nlohmann::json() const { return nlohmann::json{ {"res_msg", message}, {"id", id} }; }
        success_result_msg() {}
        success_result_msg(std::string data);

    public:
        void from_json(const nlohmann::json& json_data);
    };
}