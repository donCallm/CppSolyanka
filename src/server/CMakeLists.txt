project(server)

file(GLOB_RECURSE SERVER_CPPS ${SOURCE_DIR}/*.cpp)

target_include_directories(server PRIVATE src/logger)
target_include_directories(server PRIVATE src/utlis)
target_include_directories(server PRIVATE src/server)

add_executable(server ${SOURCE_DIR}/src/logger/logger.cpp
                                        ${SOURCE_DIR}/src/server/connect.cpp
                                        ${SOURCE_DIR}/src/server/server.cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC Boost::system)

