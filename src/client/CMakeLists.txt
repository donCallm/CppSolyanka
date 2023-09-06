project(client)

hunter_add_package(spdlog)

find_package(spdlog CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
                        client.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/client)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
