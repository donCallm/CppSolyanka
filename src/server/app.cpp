#include "app.hpp"
#include <server/hub.hpp>
#include <spdlog/spdlog.h>

namespace core
{
    app::app(int port) :
        _ioc(THREADS),
        _hub(std::make_shared<hub>(*this)),
        _port(port)
    {}

    void app::create_thread_pool()
    {
        for(auto i = 4 - 1; i > 0; --i)
        {
            _threads.emplace_back([this]{ _ioc.run(); });
        }
        _ioc.run();
    }

    void app::start()
    {
        spdlog::info("Start application");
        _hub->start();
        create_thread_pool();
    }
}
