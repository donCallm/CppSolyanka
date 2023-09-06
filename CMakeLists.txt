cmake_minimum_required(VERSION 3.21) #min version cmake

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.24.2.tar.gz"
    SHA1 "17fd3b9f0b8c271eac9b71e30a6aa1b9fecab299"
)

project(CppSolyanka) #projectname

set(CMAKE_CXX_STANDARD 17) #set c++ standart
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

hunter_add_package(Boost COMPONENTS system program_options thread context)

find_package(Boost CONFIG REQUIRED system program_options thread context)

add_subdirectory(src/client)
add_subdirectory(src/server)
