#include <iostream>
#include <spdlog/spdlog.h>
#include <fstream>

namespace utils
{
    std::ofstream _null_stream("/dev/null");
    std::streambuf* _original_cout = nullptr;

    inline void disable_console_output()
    {
        _original_cout = std::cout.rdbuf();
        std::cout.rdbuf(_null_stream.rdbuf());
        spdlog::set_level(spdlog::level::off);
    }

    inline void enable_console_ouput()
    {
        std::cout.rdbuf(_original_cout);
        spdlog::set_level(spdlog::level::info);
    }
}
