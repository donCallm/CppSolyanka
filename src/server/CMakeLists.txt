project(server)

add_subdirectory(database)
add_executable(server main.cpp
                    server.cpp
                    connect.cpp)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server/database)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server/json)

target_link_libraries(server PRIVATE spdlog::spdlog)
target_link_libraries(server PRIVATE cpp_redis::cpp_redis)
target_link_libraries(server PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(server PRIVATE database)
