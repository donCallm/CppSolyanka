project(client)

add_executable(${PROJECT_NAME} main.cpp
                        client.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/client)
