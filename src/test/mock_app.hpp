#include <gmock/gmock.h>
#include <server/app.hpp>  // Подключите ваш интерфейс

namespace tests
{
    class state;

    class mock_app : public core::app
    {
    public:
        mock_app(boost::asio::io_service& io_service);
    };
}