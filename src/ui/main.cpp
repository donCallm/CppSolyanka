#include "app.hpp"
#include <spdlog/spdlog.h>

int main(int argc, char *argv[])
{
    try
    {
        QApplication qt_app(argc, argv);
        ui::app ui_app;
        ui_app.setup();
        ui_app.start();
        return qt_app.exec();
    }
    catch(const std::exception& e)
    {
        spdlog::error(e.what());
        return EXIT_FAILURE;
    }
    catch(...)
    {
        spdlog::error("unknown exception, terminate...");
        return EXIT_FAILURE;
    }

}