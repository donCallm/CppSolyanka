project(client)

add_executable(${PROJECT_NAME} 
            main.cpp
            client.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
            spdlog::spdlog
            nlohmann_json::nlohmann_json
            objects)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../objects)