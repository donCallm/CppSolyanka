cmake_minimum_required(VERSION 3.21) #min version cmake

project(CppSolyanka) #projectname

set(CMAKE_CXX_STANDARD 17) #set c++ standart

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE CPPS ${SOURCE_DIR}/*.cpp) #GPPS - list where collect all .cpp file

include_directories(3rd/json/include)
add_executable(${PROJECT_NAME} ${CPPS}) #create exe file