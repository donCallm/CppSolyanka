project(server)

add_subdirectory(state)
add_subdirectory(database)
add_executable(server main.cpp
                    server.cpp
                    connect.cpp)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server
                                            ${CMAKE_SOURCE_DIR}/src/server/database
                                            ${CMAKE_SOURCE_DIR}/src/objects
                                            ${CMAKE_SOURCE_DIR}/src/server/state)

target_link_libraries(server PRIVATE spdlog::spdlog
                                        cpp_redis::cpp_redis
                                        nlohmann_json::nlohmann_json
                                        database
                                        objects
                                        state)
