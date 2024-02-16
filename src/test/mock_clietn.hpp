#pragma once

#include <objects/msg_objects.hpp>
#include <objects/commands.hpp>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <client/client.hpp>

namespace tests
{
    class mock_client : public core::client
    {
    public:
        mock_client();
        ~mock_client() override = default;

        MOCK_METHOD2(handler_result, void(const core::command::type& comm, const core::msg& rpl));
        MOCK_METHOD1(write, void(std::string& msg));
        MOCK_METHOD0(read_response, std::string());
        MOCK_METHOD0(connect, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD0(read_hello_msg, void());
    };
}