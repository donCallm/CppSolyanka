cmake_minimum_required(VERSION 3.26)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.24.2.tar.gz"
    SHA1 "17fd3b9f0b8c271eac9b71e30a6aa1b9fecab299"
)

project(CppSolyanka)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

hunter_add_package(Boost COMPONENTS system program_options thread context)
hunter_add_package(spdlog)
hunter_add_package(cpp_redis)
hunter_add_package(nlohmann_json)

find_package(Boost          CONFIG REQUIRED system program_options thread context)
find_package(spdlog         CONFIG REQUIRED)
find_package(cpp_redis      CONFIG REQUIRED)
find_package(nlohmann_json  CONFIG REQUIRED)
find_package(Qt5            CONFIG REQUIRED Core Widgets)

message(STATUS "Qt5 found at ${Qt5_DIR}")
message(STATUS "Qt5 Core version: ${Qt5Core_VERSION}")
message(STATUS "Qt5 Widgets version: ${Qt5Widgets_VERSION}")
message(STATUS "Qt5 include directories: ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS}")
message(STATUS "Qt5 libraries: ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}")

add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(src/objects)
add_subdirectory(src/ui)
