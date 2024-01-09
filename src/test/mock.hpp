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
            mock_client();
            ~mock_client() override = default;

        public:
            MOCK_METHOD0(read_hello_msg, void());
            MOCK_METHOD1(write, void(const std::string));
            MOCK_METHOD0(connect, void());
            MOCK_METHOD0(start, void());
            MOCK_METHOD0(executing, void());
            MOCK_METHOD2(handler_result, void(const core::command::type&, const core::msg&));
            MOCK_METHOD0(read_response, std::string());


            void mconnect();
            void mhandle_result(core::msg& rpl);
            void start(core::msg& rpl, const std::string& data);

        public:
            boost::asio::io_service _mock_io_service;
            boost::asio::ip::tcp::socket _mock_socket;
    };
}