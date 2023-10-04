project(client)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/objects ${CMAKE_BINARY_DIR}/objects)

add_executable(${PROJECT_NAME} main.cpp
                        client.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE  
    ${CMAKE_SOURCE_DIR}/src/client
    ${CMAKE_SOURCE_DIR}/src/client/json
    ${CMAKE_SOURCE_DIR}/src/objects)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog
                                            nlohmann_json::nlohmann_json
                                            objects)
