#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "commands.hpp"
#include "state.hpp"
#include "msg_objects.hpp"

namespace net
{
    class con_handler : public std::enable_shared_from_this<con_handler>
    {
        public:
            typedef std::shared_ptr<con_handler> ptr;
            explicit con_handler(boost::asio::io_service& io_service);
            ~con_handler();

        private:
            void write_message(const std::string& message);
            void handle_write(const boost::system::error_code& err, size_t byte_transferred);
            void accept_message();
            void read_message();
            void on_msg_ready();
            void say_hello();
            void lack_of_token();
            void invok_func(core::commands& comm);
            void show_result(const core::reply_msg& rpl, nlohmann::json& json_data);
            std::string get_adress();

        public:
            void start();
            static ptr create(boost::asio::io_service& io_service);
            boost::asio::ip::tcp::socket& get_socket();

        private:
            boost::asio::ip::tcp::socket _sock;
            boost::asio::streambuf _buf;
            std::vector<uint8_t> _read_buff;
            std::vector<uint8_t> _write_buff;
            std::size_t _msg_size;
            const std::string _token = "107610801084107232108310861093";
            server::state _state;
            core::user _client;
    };
}
