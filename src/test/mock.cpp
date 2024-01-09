#include "mock.hpp"

namespace client_tests
{
    mock_client::mock_client() : core::client(), _mock_socket(_mock_io_service) {}

    void mock_client::mhandle_result(core::msg& rpl)
    {
        nlohmann::json json_data = nlohmann::json::parse(rpl.message);

        if (json_data.find("err_msg") != json_data.end())
        {
            core::error_msg err;
            err.from_json(json_data);
            rpl.message = err.message;
        }
        else if (json_data.find("res_msg") != json_data.end())
        {
            core::success_result_msg res;
            res.from_json(json_data);
            rpl.message = res.message;
        }
        else
        {
            core::msg response;
            response.from_json(json_data);
            rpl.message = response.message;
        }
    }

    void mock_client::mconnect()
    {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        _mock_socket.connect(endpoint);
    }

    void mock_client::start(core::msg& rpl, const std::string& data)
    {
        read_hello_msg();
        write(data);
        rpl.from_json(nlohmann::json::parse(read_response()));
        mhandle_result(rpl);
    }

    TEST(ClientTest, testSendMessage)
    {
        testing::NiceMock<mock_client> mock_object;
        core::msg rpl;
        core::client usr;

        mock_object.mconnect();

        EXPECT_CALL(mock_object, read_hello_msg())
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::read_hello_msg));

        EXPECT_CALL(mock_object, write("{\"id\":1,\"instruction\":0,\"params\":[\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        EXPECT_CALL(mock_object, read_response())
            .Times(::testing::AtLeast(1))
            .WillRepeatedly(::testing::Invoke(&usr, &core::client::read_response));

        EXPECT_CALL(mock_object, write("{\"id\":2,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        mock_object.start(rpl, "{\"id\":1,\"instruction\":0,\"params\":[\"0\"],\"token\":\"\"}");
        mock_object.write("{\"id\":2,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}");

        ASSERT_EQ(rpl.message, "pong");

    }

    TEST(ClientTest, testSuccessRegistration)
    {
        testing::NiceMock<mock_client> mock_object;
        core::msg rpl;
        core::client usr;

        mock_object.mconnect();

        EXPECT_CALL(mock_object, read_hello_msg())
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::read_hello_msg));

        EXPECT_CALL(mock_object, write("{\"id\":3,\"instruction\":1,\"params\":[\"dontCallm\",\"egor\",\"seleznev\",\"sergeevich\",\"BM123\",\"1111\",\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        EXPECT_CALL(mock_object, read_response())
            .Times(::testing::AtLeast(1))
            .WillRepeatedly(::testing::Invoke(&usr, &core::client::read_response));
        
        EXPECT_CALL(mock_object, write("{\"id\":4,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        mock_object.start(rpl, "{\"id\":3,\"instruction\":1,\"params\":[\"dontCallm\",\"egor\",\"seleznev\",\"sergeevich\",\"BM123\",\"1111\",\"0\"],\"token\":\"\"}");
        mock_object.write("{\"id\":4,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}");

        ASSERT_EQ(rpl.message, "Successful registration");
    }

    TEST(ClientTest, testSuccessLogin)
    {
        testing::NiceMock<mock_client> mock_object;
        core::msg rpl;
        core::client usr;

        mock_object.mconnect();

        EXPECT_CALL(mock_object, read_hello_msg())
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::read_hello_msg));

        EXPECT_CALL(mock_object, write("{\"id\":5,\"instruction\":2,\"params\":[\"dontCallm\",\"1111\",\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        EXPECT_CALL(mock_object, read_response())
            .Times(::testing::AtLeast(1))
            .WillRepeatedly(::testing::Invoke(&usr, &core::client::read_response));
        
        EXPECT_CALL(mock_object, write("{\"id\":6,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        mock_object.start(rpl, "{\"id\":5,\"instruction\":2,\"params\":[\"dontCallm\",\"1111\",\"0\"],\"token\":\"\"}");
        mock_object.write("{\"id\":6,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}");

        ASSERT_EQ(rpl.message, "Successful login");
    }

    TEST(ClientTest, testClearDB)
    {
        testing::NiceMock<mock_client> mock_object;
        core::msg rpl;
        core::client usr;

        mock_object.mconnect();

        EXPECT_CALL(mock_object, read_hello_msg())
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::read_hello_msg));

        EXPECT_CALL(mock_object, write("{\"id\":7,\"instruction\":13,\"params\":[\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        EXPECT_CALL(mock_object, read_response())
            .Times(::testing::AtLeast(1))
            .WillRepeatedly(::testing::Invoke(&usr, &core::client::read_response));
        
        EXPECT_CALL(mock_object, write("{\"id\":8,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}"))
            .Times(::testing::AtLeast(1))
            .WillOnce(::testing::Invoke(&usr, &core::client::write));

        mock_object.start(rpl, "{\"id\":7,\"instruction\":13,\"params\":[\"0\"],\"token\":\"\"}");
        mock_object.write("{\"id\":8,\"instruction\":12,\"params\":[\"0\"],\"token\":\"\"}");

        ASSERT_EQ(rpl.message, "Successful database cleanup");
    }
}