#include "app.hpp"
#include <server/state/state.hpp>
#include <server/hub.hpp>
#include <spdlog/spdlog.h>
#include <server/database/database.hpp>

namespace core
{
    app::app(boost::asio::io_service& io_service) :
        _io_service(io_service),
        _hub(std::make_shared<hub>(*this)),
        _state(std::make_shared<state>())
    {
        spdlog::info("Start application");
        db::database::get_instance();
        _hub->start();
    }
}
