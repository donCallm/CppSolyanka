#pragma once

#include <objects/msg_objects.hpp>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <client/client.hpp>

namespace client_tests
{
    class mock_client : public core::client
    {
        public:
            mock_client(bool console_mode = true) : core::client(console_mode) {}
            ~mock_client() {}

        public:
            MOCK_METHOD0(read_hello_msg, void());
            MOCK_METHOD1(write, void(std::string));
            MOCK_METHOD0(connect, void());
            MOCK_METHOD0(start, void());
            MOCK_METHOD2(handler_result, void(const core::command::type&, const core::msg&));
            MOCK_METHOD0(read_response, std::string());
            MOCK_METHOD0(get_socket, boost::asio::ip::tcp::socket*());

            void mwrite(const std::string& data);
            void mhandle_result(core::msg& rpl);
            void start_process(const std::string& data, core::msg& rpl);
    };
}