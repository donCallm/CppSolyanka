#include "mock_clietn.hpp"
#include "mock_hub.hpp"
#include "utils.hpp"

namespace tests
{
    mock_client::mock_client() : core::client(), _mock_socket(_mock_io_service) {}

    mock_hub::mock_hub(core::app& application, bool constructor_flag) : core::hub(application, constructor_flag) {}

    TEST(ServerTest, testRegistration)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        comm.set_command("registration dontCallm egor seleznev sergeevich BM123 1111 0");

        EXPECT_CALL(mhub, handle_create_user(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_user(comm); }));

        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":3,\"params\":[],\"res_msg\":\"Successful registration\"}");
    }

    TEST(ServerTest, testIncorrectRegistration)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

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
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        comm.set_command("login dontCallm 1111 0");

        EXPECT_CALL(mhub, handle_login(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_login(comm); }));

        rpl = mhub.handle_login(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":15,\"params\":[],\"res_msg\":\"Successful login\"}");
    }

    TEST(ServerTest, testIncorrectLogin)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

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

    TEST(ServerTest, testCreateBankAcc)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        comm.set_command("create_bank_account 0");

        EXPECT_CALL(mhub, handle_create_bank_acc(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_bank_acc(comm); }));

        rpl = mhub.handle_create_bank_acc(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":31,\"params\":[],\"res_msg\":\"Successful create new bank account\"}");
    }

    TEST(ServerTest, testIncorrectCreateBankAcc)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        EXPECT_CALL(mhub, handle_create_bank_acc(::testing::_))
            .WillRepeatedly(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_bank_acc(comm); }));

        //wrong numbers of params
        comm.set_command("create_bank_account 0 0");
        rpl = mhub.handle_create_bank_acc(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":34,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("create_bank_account !!!!");
        rpl = mhub.handle_create_bank_acc(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Error. ID would be number\",\"id\":36,\"params\":[]}");
        comm.params.clear();
        //Wrong id
        comm.set_command("create_bank_account 100");
        rpl = mhub.handle_create_bank_acc(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Error. User not found\",\"id\":38,\"params\":[]}");
    }

    TEST(ServerTest, testCreateCard)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        comm.set_command("create_card 0 0");

        EXPECT_CALL(mhub, handle_create_card(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_card(comm); }));

        rpl = mhub.handle_create_card(comm).value();

        ASSERT_EQ(rpl.message, "{\"id\":41,\"params\":[],\"res_msg\":\"Successful created\"}");
    }

    TEST(ServerTest, testIncorrectCreateCar)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        EXPECT_CALL(mhub, handle_create_card(::testing::_))
            .WillRepeatedly(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_create_card(comm); }));

        //wrong numbers of params
        comm.set_command("create_bank_account 0 0 0");
        rpl = mhub.handle_create_card(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":44,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("create_bank_account !!!! !!!!");
        rpl = mhub.handle_create_card(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only numbers can be used\",\"id\":46,\"params\":[]}");
        comm.params.clear();
        //Wrong id
        comm.set_command("create_bank_account 0 100");
        rpl = mhub.handle_create_card(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"User not found\",\"id\":48,\"params\":[]}");
    }

    TEST(ServerTest, testChangeBalance)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        EXPECT_CALL(mhub, handle_change_balance(::testing::_))
            .WillRepeatedly(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_change_balance(comm); }));

        comm.set_command("replenish_balance 0 100 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":51,\"params\":[],\"res_msg\":\"Successful change balance\"}");
        comm.params.clear();

        comm.set_command("debit_funds 0 50 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":53,\"params\":[],\"res_msg\":\"Successful change balance\"}");
    }

    TEST(ServerTest, testIncorrectChangeBalance)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        EXPECT_CALL(mhub, handle_change_balance(::testing::_))
            .WillRepeatedly(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_change_balance(comm); }));

        //wrong numbers of params
        comm.set_command("replenish_balance 0 0 0 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":56,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("replenish_balance !!!! !!!! !!!!");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only numbers can be used\",\"id\":58,\"params\":[]}");
        comm.params.clear();
        //Wrong id
        comm.set_command("replenish_balance 0 0 100");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"User not found\",\"id\":60,\"params\":[]}");
        comm.params.clear();
        //Wrong card id
        comm.set_command("replenish_balance 100 0 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Card not found\",\"id\":62,\"params\":[]}");
        comm.params.clear();
        //Wrong sum
        comm.set_command("debit_funds 0 1000 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Amount is greater than balance\",\"id\":64,\"params\":[]}");
    }
}