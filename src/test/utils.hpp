#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include "../server/database/database.hpp"
using namespace db;

namespace utils
{
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
