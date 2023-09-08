project(server)

add_executable(server main.cpp
                            server.cpp
                            connect.cpp)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
