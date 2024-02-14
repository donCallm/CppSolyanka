project(AppUI)

add_executable(${PROJECT_NAME} 
            app.cpp
            main.cpp
            api/api.cpp
            authorization/auth.cpp)

target_link_libraries(${PROJECT_NAME}
            objects
            lclient
            spdlog::spdlog 
            Qt5::Core 
            Qt5::Widgets)
