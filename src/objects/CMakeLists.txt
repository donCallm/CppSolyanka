project(objects)

add_library(objects STATIC objects.cpp)

target_link_libraries(objects PRIVATE nlohmann_json::nlohmann_json)
