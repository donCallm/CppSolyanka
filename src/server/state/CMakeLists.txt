project(state)

add_library(state STATIC 
            state.cpp)

target_link_libraries(state PRIVATE 
            database
            objects
            nlohmann_json::nlohmann_json)

