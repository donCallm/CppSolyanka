#include "mock.hpp"

namespace client_tests
{
    void mock_client::mwrite(const std::string& data)
    {
        core::command comm;

        comm.set_command(data);
        comm.params.push_back("0"); //id for tests

        nlohmann::json serialize_message = comm; 
        std::string json_string = serialize_message.dump();
        std::cout << json_string << std::endl;
        write(json_string);
    }

    void mock_client::mhandle_result(core::msg& rpl)
    {
        std::cout << rpl.message << std::endl;
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

    void mock_client::start_process(const std::string& data, core::msg& rpl)
    {
        mwrite(data);
        rpl.from_json(nlohmann::json::parse(read_response()));
        mhandle_result(rpl);
    }

    TEST(ClientTest, testSendMessage)
    {
        mock_client mclient;
        core::msg rpl;

        EXPECT_CALL(mclient, write("{\"id\":2,\"instruction\":0,\"params\":[\"0\"],\"token\":\"\"}")); //expected parameters
        EXPECT_CALL(mclient, read_response())
            .WillOnce(testing::Return("{\"id\":1,\"message\":\"{\\\"id\\\":1,\\\"message\\\":\\\"pong\\\",\\\"params\\\":[]}\",\"params\":[]}")); //expected parameters

        mclient.start_process("ping", rpl);
        ASSERT_EQ("pong", rpl.message);
    }

    TEST(ClientTest, testSuccessRegistration)
    {
        mock_client mclient;
        core::msg rpl;

        EXPECT_CALL(mclient, write("{\"id\":4,\"instruction\":1,\"params\":[\"dontCallm\",\"egor\",\"seleznev\",\"sergeevich\",\"BM123\",\"1111\",\"0\"],\"token\":\"\"}")); //expected parameters
        EXPECT_CALL(mclient, read_response())
            .WillOnce(testing::Return("{\"id\":3,\"message\":\"{\\\"id\\\":3,\\\"params\\\":[],\\\"res_msg\\\":\\\"Successful registration\\\"}\",\"params\":[]}")); //expected parameters

        mclient.start_process("registration dontCallm egor seleznev sergeevich BM123 1111", rpl);
        ASSERT_EQ("Successful registration", rpl.message);
    }
}