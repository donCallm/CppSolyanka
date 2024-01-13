#include "mock_clietn.hpp"
#include "mock_hub.hpp"

namespace tests
{
    mock_client::mock_client() : core::client(), _mock_socket(_mock_io_service) {}

    TEST(ClientTest, testSuccessRegistration)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;

        comm.set_command("registration dontCallm egor seleznev sergeevich BM123 1111 0");

        EXPECT_CALL(mhub, handle_create_user(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_user(comm); }))
            .WillRepeatedly(::testing::Return(std::nullopt));

        rpl = mhub.handle_create_user(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":3,\"params\":[],\"res_msg\":\"Successful registration\"}");
    }

    TEST(ClientTest, testLogin)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;

        comm.set_command("login dontCallm 1111 0");

        EXPECT_CALL(mhub, handle_login(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_login(comm); }))
            .WillRepeatedly(::testing::Return(std::nullopt));

        rpl = mhub.handle_login(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":6,\"params\":[],\"res_msg\":\"Successful login\"}");
    }

    TEST(ClientTest, testClearDB)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;

        comm.set_command("clear_databases 0");

        EXPECT_CALL(mhub, handler_clear_dbs(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handler_clear_dbs(comm); }))
            .WillRepeatedly(::testing::Return(std::nullopt));

        rpl = mhub.handler_clear_dbs(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":9,\"params\":[],\"res_msg\":\"Successful database cleanup\"}");
    }
}