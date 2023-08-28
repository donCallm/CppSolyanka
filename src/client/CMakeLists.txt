project(client)

include_directories(${CMAKE_SOURCE_DIR}/src/client)

add_executable(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src/client/main.cpp
                        ${CMAKE_SOURCE_DIR}/src/client/client.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/client)

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::system)

add_subdirectory(src/client/commands)