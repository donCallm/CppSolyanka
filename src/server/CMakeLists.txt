project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -lgcov")

add_subdirectory(state)
add_subdirectory(database)

add_library(lserver
            app.cpp
            connect.cpp
            hub.cpp
            server.cpp)

add_executable(server
            main.cpp
            server.cpp
            connect.cpp
            hub.cpp
            app.cpp)

target_link_libraries(lserver PRIVATE
            spdlog::spdlog
            cpp_redis::cpp_redis
            nlohmann_json::nlohmann_json
            database
            objects
            state)

target_link_libraries(server PRIVATE   
            spdlog::spdlog
            cpp_redis::cpp_redis
            nlohmann_json::nlohmann_json
            database
            objects
            state)
