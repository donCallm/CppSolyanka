project(commands)

include_directories(${CMAKE_SOURCE_DIR}/src/logger)
include_directories(${CMAKE_SOURCE_DIR}/src/client)

add_library(${PROJECT_NAME} STATIC commands.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src/client/commands)

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::system)
