#pragma once

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../client/client.hpp"

namespace client_tests
{
    class mock_client : public core::client
    {
        public:
            mock_client(bool console_mode = true);
            ~mock_client() override {}

        public:
            MOCK_METHOD0(read_hello_msg, void());
            MOCK_METHOD1(write, void(std::string));
            MOCK_METHOD0(connect, void());
            MOCK_METHOD0(start, void());
            MOCK_METHOD2(handler_result, void(const core::command::type&, const core::msg&));
            MOCK_METHOD0(read_response, std::string());
            MOCK_METHOD0(get_socket, boost::asio::ip::tcp::socket*());
    };
}