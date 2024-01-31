#include "mock_clietn.hpp"
#include "mock_hub.hpp"
#include "mock_app.hpp"
#include "utils.hpp"

namespace tests
{
    mock_client::mock_client(bool console_mode) : core::client(console_mode), _mock_socket(_mock_io_service) {}

    mock_hub::mock_hub(core::app& application) : core::hub(application) {}

    mock_app::mock_app(boost::asio::io_service& io_service) : core::app(io_service) {}

    TEST(ClientTest, Connect)
    {
        boost::asio::io_service io_service;
        mock_app mapp(io_service);
        std::thread threadIoService([&]{ io_service.run(); });

        testing::NiceMock<mock_client> mclient(true);

        EXPECT_CALL(mclient, connect())
            .WillOnce(::testing::Invoke([&mclient]() { mclient.connect(); }));

        io_service.stop();

        threadIoService.join();
    }

    TEST(ServerTest, Registration)
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
        ASSERT_EQ(rpl.message, "{\"id\":8,\"params\":[],\"res_msg\":\"Successful registration\"}");
    }

    TEST(ServerTest, IncorrectRegistration)
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
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"User already exists1\",\"id\":11,\"params\":[]}");
        comm.params.clear();
        //wrong numbers of params
        comm.set_command("registration dontCallm egor seleznev sergeevich BM123 1111 0 0");
        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":13,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("registration !!!! egor seleznev sergeevich BM123 !!!! 0");
        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only letters and numbers can be used\",\"id\":15,\"params\":[]}");
        comm.params.clear();
        //invalid symbol in name, patranymic, sername
        comm.set_command("registration dontCallm eg0r s0leznev s0rgeevich BM123 1111 0");
        rpl = mhub.handle_create_user(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Last name, first name and patronymic can only have letters\",\"id\":17,\"params\":[]}");
    }

    TEST(ServerTest, Login)
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

        ASSERT_EQ(rpl.message, "{\"id\":20,\"params\":[],\"res_msg\":\"Successful login\"}");
    }

    TEST(ServerTest, IncorrectLogin)
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
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":23,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("login dontCallm !!!! 0");
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only letters and numbers can be used\",\"id\":25,\"params\":[]}");
        comm.params.clear();
        //invalid login
        comm.set_command("login dontcallm 1111 0");
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Error of login\",\"id\":27,\"params\":[]}");
        comm.params.clear();
        //invalid password
        comm.set_command("login dontCallm 0000 0");
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong password\",\"id\":29,\"params\":[]}");
        comm.params.clear();
        //re-authorization
        comm.set_command("login dontCallm 1111 0");
        mhub.handle_login(comm);
        rpl = mhub.handle_login(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"The user is already authorized\",\"id\":33,\"params\":[]}");
    }

    TEST(ServerTest, CreateBankAcc)
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

        ASSERT_EQ(rpl.message, "{\"id\":36,\"params\":[],\"res_msg\":\"Successful create new bank account\"}");
    }

    TEST(ServerTest, IncorrectCreateBankAcc)
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
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":39,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("create_bank_account !!!!");
        rpl = mhub.handle_create_bank_acc(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Error. ID would be number\",\"id\":41,\"params\":[]}");
        comm.params.clear();
        //Wrong id
        comm.set_command("create_bank_account 100");
        rpl = mhub.handle_create_bank_acc(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Error. User not found\",\"id\":43,\"params\":[]}");
    }

    TEST(ServerTest, CreateCard)
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

        ASSERT_EQ(rpl.message, "{\"id\":46,\"params\":[],\"res_msg\":\"Successful created\"}");
    }

    TEST(ServerTest, IncorrectCreateCard)
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
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":49,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("create_bank_account !!!! !!!!");
        rpl = mhub.handle_create_card(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only numbers can be used\",\"id\":51,\"params\":[]}");
        comm.params.clear();
        //Wrong id
        comm.set_command("create_bank_account 0 100");
        rpl = mhub.handle_create_card(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"User not found\",\"id\":53,\"params\":[]}");
    }

    TEST(ServerTest, ChangeBalance)
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
        ASSERT_EQ(rpl.message, "{\"id\":56,\"params\":[],\"res_msg\":\"Successful change balance\"}");
        comm.params.clear();

        comm.set_command("debit_funds 0 50 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":58,\"params\":[],\"res_msg\":\"Successful change balance\"}");
    }

    TEST(ServerTest, IncorrectChangeBalance)
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
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Wrong numbers of parametrs\",\"id\":61,\"params\":[]}");
        comm.params.clear();
        //invalid symmbol
        comm.set_command("replenish_balance !!!! !!!! !!!!");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Invalid symmbol. Only numbers can be used\",\"id\":63,\"params\":[]}");
        comm.params.clear();
        //Wrong id
        comm.set_command("replenish_balance 0 0 100");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"User not found\",\"id\":65,\"params\":[]}");
        comm.params.clear();
        //Wrong card id
        comm.set_command("replenish_balance 100 0 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Card not found\",\"id\":67,\"params\":[]}");
        comm.params.clear();
        //Wrong sum
        comm.set_command("debit_funds 0 1000 0");
        rpl = mhub.handle_change_balance(comm).value();
        ASSERT_EQ(rpl.message, "{\"err_msg\":\"Amount is greater than balance\",\"id\":69,\"params\":[]}");
    }

    TEST(ServerTest, GetBankInfo)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        EXPECT_CALL(mhub, handle_get_bank_info(::testing::_))
            .WillRepeatedly(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handle_get_bank_info(comm); }));

        comm.set_command("get_balance 0 0");
        rpl = mhub.handle_get_bank_info(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":72,\"params\":[],\"res_msg\":\"Your balance: 50\"}");
        comm.params.clear();

        comm.set_command("get_cards 0");
        rpl = mhub.handle_get_bank_info(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":74,\"params\":[],\"res_msg\":\"Id ur cards: 0 \"}");
        comm.params.clear();

        comm.set_command("get_bank_accounts 0");
        rpl = mhub.handle_get_bank_info(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":76,\"params\":[],\"res_msg\":\"Id ur bank accounts: 0 \"}");
        comm.params.clear();
    }

    TEST(ServerTest, GetInfo)
    {
        utils::disable_console_output();
        boost::asio::io_service io_service;
        core::app application(io_service);
        testing::NiceMock<mock_hub> mhub(application);
        core::msg rpl;
        core::command comm;
        utils::enable_console_output();

        EXPECT_CALL(mhub, handler_get_info(::testing::_))
            .WillOnce(::testing::Invoke([&mhub](core::command& comm) { return mhub.core::hub::handler_get_info(comm); }));

        comm.set_command("get_info 0");
        rpl = mhub.handler_get_info(comm).value();
        ASSERT_EQ(rpl.message, "{\"id\":79,\"params\":[],\"res_msg\":\"{\\\"bank_accounts\\\":[0],"
            "\\\"cards\\\":[0],\\\"id\\\":0,\\\"login\\\":\\\"dontCallm\\\",\\\"name\\\":\\\"egor\\\","
            "\\\"pasport\\\":\\\"BM123\\\",\\\"password\\\":\\\"1111\\\",\\\"patronymic\\\":\\\"\\\","
            "\\\"surname\\\":\\\"seleznev\\\"}\"}");
    }
}