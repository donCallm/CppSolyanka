#include <iostream>
#include "utils.hpp"
#include "mock_clietn.hpp"
#include "mock_hub.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    
    utils::clear_dbs();

    return RUN_ALL_TESTS();
}