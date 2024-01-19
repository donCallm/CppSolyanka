#pragma once

#include <iostream>
#include <spdlog/spdlog.h>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include "../server/database/database.hpp"
using namespace db;

namespace utils
{
    inline std::ofstream _null_stream("/dev/null");
    inline std::streambuf* _original_cout = nullptr;

    inline void disable_console_output()
    {
        _original_cout = std::cout.rdbuf();
        std::cout.rdbuf(_null_stream.rdbuf());
        spdlog::set_level(spdlog::level::off);
    }

    inline void enable_console_output()
    {
        std::cout.rdbuf(_original_cout);
        spdlog::set_level(spdlog::level::info);
    }

    inline void clear_dbs()
    {
        try
        {
            std::filesystem::remove(std::filesystem::current_path().remove_filename().generic_string() + "server/state/logins.json");
            db::database::get_instance()->clear_databases();
        }
        catch(const std::exception& e)
        {
            std::runtime_error("Unable to open file for reading");
        }
    }
}
