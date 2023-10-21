#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace core
{
    class reply
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"msg", msg}, {"params", params} }; }
            
            enum error { wrong_params = 0,
                        wrong_pass = 1,
                        already_exist = 2,
                        already_authorized = 3,
                        wrong_pasport = 4
                        };

            enum type { successful_registration = 5,
                        successful_logged = 6,
                        ping = 7,
                        uncknow_command = 8
                        };

        public:
            void from_json(const nlohmann::json& json_data);

        public:
            static const std::unordered_map<core::reply::type, std::string> reply_messages;
            static const std::unordered_map<core::reply::error, std::string> error_messages;
            error err;
            type msg;
            std::vector<std::string> params;
    };
}
