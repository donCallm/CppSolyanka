project(state)

add_library(state STATIC state.cpp)

target_include_directories(state PRIVATE ${CMAKE_SOURCE_DIR}/src/server/database
                                            ${CMAKE_SOURCE_DIR}/src/objects)

target_link_libraries(state PRIVATE database
                                    objects
                                    nlohmann_json::nlohmann_json
                                    spdlog::spdlog)
