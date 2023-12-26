project(objects)

# все либы надо пометить как lobjects например пример в клиенте есть
add_library(objects STATIC 
            objects.cpp
            msg_objects.cpp
            finance.cpp)

target_link_libraries(objects PUBLIC
            nlohmann_json::nlohmann_json
            database)
