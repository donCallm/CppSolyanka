project(client)

file(GLOB_RECURSE CLIENT_CPPS ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(client ${CLIENT_CPPS})

target_link_libraries(client PUBLIC Boost::system)
