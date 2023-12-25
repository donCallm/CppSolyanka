#include "mock.hpp"

namespace client_tests
{
    TEST(ClientTest, CommandSendingTest)
    {
        mock_client mclient;
        core::command comm;

        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 8080);
        mclient.get_socket()->connect(endpoint);

        comm.set_command("ping");
        comm.params.push_back(0);
        nlohmann::json serialize_message = comm; 
        std::string json_string = serialize_message.dump();

        mclient.write(json_string);

        ASSERT_EQ("pong", mclient.read_response());
    }
}