project(client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -lgcov")

add_library(lclient client.cpp)

add_executable(client 
            main.cpp
            client.cpp)
            
target_link_libraries(lclient PRIVATE
            gcov
            spdlog::spdlog
            nlohmann_json::nlohmann_json
            objects)

target_link_libraries(client PRIVATE
            gcov
            spdlog::spdlog
            nlohmann_json::nlohmann_json
            objects)