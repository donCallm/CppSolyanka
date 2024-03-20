project(objects)

add_library(objects STATIC 
            objects.cpp
            msg_objects.cpp
            finance.cpp
            tx.cpp
            binder.cpp
            mempool.cpp)

target_link_libraries(objects PUBLIC
            nlohmann_json::nlohmann_json
            database
            state)
