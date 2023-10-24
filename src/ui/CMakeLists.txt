project(AppUI)

add_executable(${PROJECT_NAME} 
                main.cpp
                authorization/auth.hpp
                authorization/auth.cpp)

target_include_directories(AppUI PRIVATE authorization)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog Qt5::Core Qt5::Widgets)
