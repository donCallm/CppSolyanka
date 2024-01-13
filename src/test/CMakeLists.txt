project(tests)

add_executable(${PROJECT_NAME} 
            main.cpp
            mock.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
            lclient
            lserver
            objects
            nlohmann_json::nlohmann_json
            GTest::gtest
            GTest::gtest_main
            GTest::gmock_main)