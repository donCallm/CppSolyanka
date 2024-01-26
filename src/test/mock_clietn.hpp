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
        mock_client(bool console_mode);
        ~mock_client() override = default;

        MOCK_METHOD0(connect, void());
        MOCK_METHOD0(stop, void());
    public:
        boost::asio::io_service _mock_io_service;
        boost::asio::ip::tcp::socket _mock_socket;
    };
}