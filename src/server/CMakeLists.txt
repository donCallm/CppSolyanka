project(server)

add_subdirectory(state)
add_subdirectory(database)

add_executable(server 
            main.cpp
            server.cpp
            connect.cpp)

target_link_libraries(server PRIVATE    
            spdlog::spdlog
            cpp_redis::cpp_redis
            nlohmann_json::nlohmann_json
            database
            objects
            state)
