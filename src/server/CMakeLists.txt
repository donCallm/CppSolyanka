project(server)

hunter_add_package(spdlog)

find_package(spdlog CONFIG REQUIRED)

add_executable(server main.cpp
                            server.cpp
                            connect.cpp)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
