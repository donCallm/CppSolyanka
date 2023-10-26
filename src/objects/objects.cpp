#include <objects/commands.hpp>
#include <objects/user.hpp>
#include <objects/message.hpp>
#include <boost/tokenizer.hpp>

namespace core
{
    uint64_t command::id = 0;

    const std::unordered_map<std::string, command::type> command::command_map = {
                {"ping", command::type::ping},
                {"login", command::type::login},
                {"registration", command::type::registration},
                {"end", command::type::end}
            };
            
    message deserialize_message(const std::vector<uint8_t>& msg_buff, std::size_t size)
    {
        message msg;

        if (msg_buff.size() < size) throw std::runtime_error("Not enought data for deserialize");

        msg.data.resize(size);
        std::memcpy(msg.data.data(), msg_buff.data(), size);

        return msg;
    }

    std::vector<uint8_t> serialize_message(const message& msg)
    {
        header h;
        h.msg_size = msg.data.size();

        std::vector<uint8_t> buffer(sizeof(std::size_t) + msg.data.size());

        std::memcpy(buffer.data(), &h.msg_size, sizeof(std::size_t));
        std::memcpy(buffer.data() + sizeof(std::size_t), msg.data.data(), msg.data.size());

        return buffer;
    }

    void command::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");

        json_data.at("id").get_to(id);
        json_data.at("instruction").get_to(instruction);
        json_data.at("params").get_to(params);
        json_data.at("token").get_to(token);
    }

    void command::set_command(const std::string& input)
    { 
        if (input.empty()) throw std::runtime_error("Empty message for set command");
        std::vector<std::string> result;
        std::istringstream iss(input);
        std::string token;
        std::getline(iss, token, ' ');

        auto it = command_map.find(token);
        if (it != command_map.end()) 
            instruction = it->second;
        else
            instruction = unknown_command;
        
        while (std::getline(iss, token, ' ')) { params.push_back(token); }
        id++;
    }

    void user::from_json(const nlohmann::json& json_data)
    {
        if (json_data.empty()) throw std::runtime_error("Empty json");
        
        json_data.at("pasport").get_to(pasport);
        json_data.at("name").get_to(name);
        json_data.at("surname").get_to(surname);
        json_data.at("id").get_to(id);
        json_data.at("password").get_to(password);
    }

}