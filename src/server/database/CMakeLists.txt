project(database)

add_library(database STATIC database.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE cpp_redis::cpp_redis)
