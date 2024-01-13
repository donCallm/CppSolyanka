#include <iostream>
#include "mock_clietn.hpp"
#include "mock_hub.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
  
    return RUN_ALL_TESTS();
}