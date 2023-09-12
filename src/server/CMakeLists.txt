project(server)

add_executable(server main.cpp
                    server.cpp
                    connect.cpp
                    database/database.cpp)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server/database)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

add_subdirectory(database)
