#include <iostream>
#include "tests.hpp"

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
  
    return RUN_ALL_TESTS();
}