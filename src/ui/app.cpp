#include "app.hpp"
#include <spdlog/spdlog.h>

namespace ui
{

app::app() 
{
    spdlog::info("Create app");
}

int app::start()
{
    _lw->show();
}

void app::setup()
{
    spdlog::info("Setup app");
    _lw = std::make_shared<login_window>();
}

}