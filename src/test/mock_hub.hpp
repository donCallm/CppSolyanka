#pragma once

#include <objects/msg_objects.hpp>
#include <objects/commands.hpp>
#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <server/hub.hpp>
#include <server/app.hpp>
#include <memory>
using namespace net;

namespace tests
{
    class mock_hub : public core::hub
    {
    public:
        mock_hub(core::app& application);
        ~mock_hub() override = default;

        MOCK_METHOD1(handle_create_user, std::optional<core::msg>(core::command& comm));
        MOCK_METHOD1(handle_login, std::optional<core::msg>(core::command& comm));
        MOCK_METHOD1(handle_create_bank_acc, std::optional<core::msg>(core::command& comm));
        MOCK_METHOD1(handle_change_balance, std::optional<core::msg>(core::command& comm));
        MOCK_METHOD1(handle_create_card, std::optional<core::msg>(core::command& comm));
        MOCK_METHOD1(handle_get_bank_info, std::optional<core::msg>(core::command& comm));
        MOCK_METHOD1(handler_get_info, std::optional<core::msg>(core::command& comm));
        MOCK_METHOD1(handler_clear_dbs, std::optional<core::msg>(core::command& comm));
    };
}