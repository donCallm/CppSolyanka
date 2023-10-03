#pragma once

#include <iostream>
#include <nlohmann/json.hpp>

namespace objects
{
    class commands
    {
        public:
            operator nlohmann::json() const { return nlohmann::json{ {"instruction", instruction}, {"params", params}, {"id", id}, {"token", token} }; }

        public:
            void from_json(const nlohmann::json& json_data)
            {
                json_data.at("instruction").get_to(instruction);
                json_data.at("params").get_to(params);
                json_data.at("id").get_to(id);
                json_data.at("token").get_to(token);
            }

            void get_id()
            {
                id = last_id++;
            }

            void set_command(const std::string& input)
            { 
                std::vector<std::string> result;
                std::istringstream iss(input);
                std::string token;
                std::getline(iss, token, ' ');
                instruction = token;
                while (std::getline(iss, token, ' ')) { params.push_back(token); }
                get_id();
            }

        public:
            std::string instruction;
            std::vector<std::string> params;
            std::string token;
            int id;
            static int last_id;
    };

    int commands::last_id = 0;
}
