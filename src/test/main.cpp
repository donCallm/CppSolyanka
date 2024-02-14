#include <iostream>
#include "utils.hpp"
#include "mock_clietn.hpp"
#include "mock_hub.hpp"
#include <spdlog/spdlog.h>

int main(int argc, char **argv)
{
    spdlog::set_level(spdlog::level::off);
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    
    utils::clear_dbs();

    return RUN_ALL_TESTS();
}