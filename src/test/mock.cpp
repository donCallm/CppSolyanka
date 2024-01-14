#include "mock_clietn.hpp"
#include "mock_hub.hpp"

namespace tests
{
    mock_client::mock_client() : core::client(), _mock_socket(_mock_io_service) {}

    TEST(ServerTest, testSuccessRegistration)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        comm.set_command("registration dontCallm egor seleznev sergeevich BM123 1111 0");

        EXPECT_CALL(mhub, handle_create_user(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_user(comm); }));

        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":3,\"params\":[],\"res_msg\":\"Successful registration\"}");
    }

    TEST(ServerTest, testIncorrectRegistration)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;

        EXPECT_CALL(mhub, handle_create_user(::testing::_))
            .WillRepeatedly(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_user(comm); }));

        //re-registration
        comm.set_command("registration dontCallm egor seleznev sergeevich BM123 1111 0");
        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"User already exists1\",\"id\":6,\"params\":[]}");
        comm.params.clear();
        //wrong numbers of params
        comm.set_command("registration dontCallm egor seleznev sergeevich BM123 1111 0 0");
        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":8,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("registration !!!! egor seleznev sergeevich BM123 !!!! 0");
        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only letters and numbers can be used\",\"id\":10,\"params\":[]}");
        comm.params.clear();
        //invalid symbol in name, patranymic, sername
        comm.set_command("registration dontCallm eg0r s0leznev s0rgeevich BM123 1111 0");
        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Last name, first name and patronymic can only have letters\",\"id\":12,\"params\":[]}");
    }

    TEST(ServerTest, testLogin)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;

        comm.set_command("login dontCallm 1111 0");

        EXPECT_CALL(mhub, handle_login(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_login(comm); }));

        rpl = mhub.handle_login(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":15,\"params\":[],\"res_msg\":\"Successful login\"}");
    }

    TEST(ServerTest, testIncorrectLogin)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;

        EXPECT_CALL(mhub, handle_login(::testing::_))
            .WillRepeatedly(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_login(comm); }));

        //wrong numbers of params
        comm.set_command("login dontCallm 1111 0 0");
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":18,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("login dontCallm !!!! 0");
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only letters and numbers can be used\",\"id\":20,\"params\":[]}");
        comm.params.clear();
        //invalid login
        comm.set_command("login dontcallm 1111 0");
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Error of login\",\"id\":22,\"params\":[]}");
        comm.params.clear();
        //invalid password
        comm.set_command("login dontCallm 0000 0");
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong password\",\"id\":24,\"params\":[]}");
        comm.params.clear();
        //re-authorization
        comm.set_command("login dontCallm 1111 0");
        mhub.handle_login(comm);
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"The user is already authorized\",\"id\":28,\"params\":[]}");
    }

    TEST(ServerTest, testClearDB)
    {
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;

        comm.set_command("clear_databases 0");

        EXPECT_CALL(mhub, handler_clear_dbs(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handler_clear_dbs(comm); }));

        rpl = mhub.handler_clear_dbs(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":31,\"params\":[],\"res_msg\":\"Successful database cleanup\"}");
    }
}