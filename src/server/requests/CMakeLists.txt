project(requests)

add_library(requests request_methods.cpp)

target_link_libraries(requests PRIVATE nlohmann_json::nlohmann_json)
