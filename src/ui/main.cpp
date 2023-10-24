#include "authorization/auth.hpp"
#include <spdlog/spdlog.h>

int main(int argc, char *argv[])
{
    try
    {
        QApplication app(argc, argv);
        ui::app main_window;
        main_window.setup();
        main_window.start();
        return app.exec();
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