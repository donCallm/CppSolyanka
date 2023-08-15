project(server)

include_directories(${CMAKE_SOURCE_DIR}/3rd/json/include)
include_directories(${CMAKE_SOURCE_DIR}/src/server) 
include_directories(${CMAKE_SOURCE_DIR}/src/logger)
include_directories(${CMAKE_SOURCE_DIR}/src/utlis)

add_executable(server ${CMAKE_SOURCE_DIR}/src/server/main.cpp
                    ${CMAKE_SOURCE_DIR}/src/logger/logger.cpp
                    ${CMAKE_SOURCE_DIR}/src/server/server.cpp
                    ${CMAKE_SOURCE_DIR}/src/server/connect.cpp)

target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/logger)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/utlis)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/server)

target_link_libraries(${PROJECT_NAME} PUBLIC Boost::system)

