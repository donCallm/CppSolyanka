project(tests)

add_executable(${PROJECT_NAME} 
            main.cpp
            tests.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
            objects
            clib
            nlohmann_json::nlohmann_json
            GTest::gtest
            GTest::gtest_main
            GTest::gmock_main)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../client)
