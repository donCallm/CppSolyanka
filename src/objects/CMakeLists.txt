project(objects)

add_library(${PROJECT_NAME} STATIC 
            objects.cpp
            msg_objects.cpp
            finance.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
            nlohmann_json::nlohmann_json
            database)
