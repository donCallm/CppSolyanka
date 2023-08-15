cmake_minimum_required(VERSION 3.21) #min version cmake

project(CppSolyanka) #projectname

find_package(Boost REQUIRED COMPONENTS system)

set(CMAKE_CXX_STANDARD 17) #set c++ standart
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src/client)
add_subdirectory(src/server)
