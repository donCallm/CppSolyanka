project(database)

find_library(HIREDIS_LIB hiredis)
message("HIREDIS_LIB - " ${HIREDIS_LIB})
find_library(REDIS_PLUS_PLUS_LIB redis++)
message("REDIS_PLUS_PLUS_LIB - " ${REDIS_PLUS_PLUS_LIB})

target_include_directories(database PRIVATE ${CMAKE_SOURCE_DIR}/src/server/database)