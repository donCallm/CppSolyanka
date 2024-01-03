project(client)

add_library(lclient client.cpp)

add_executable(client 
            main.cpp
            client.cpp)

target_link_libraries(lclient PRIVATE 
            spdlog::spdlog
            nlohmann_json::nlohmann_json
            objects)

target_link_libraries(client PRIVATE 
            spdlog::spdlog
            nlohmann_json::nlohmann_json
            objects)