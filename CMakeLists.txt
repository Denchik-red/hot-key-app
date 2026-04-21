cmake_minimum_required(VERSION 3.10)
project(hot-key-app)

set(CMAKE_CXX_STANDARD 17)

find_package(X11 REQUIRED)

add_executable(app-linux main-linux.cpp)
target_link_libraries(app-linux PRIVATE ${X11_LIBRARIES})

add_executable(app-windows main-windows.cpp)
target_link_libraries(app-windows PRIVATE user32 gdi32)
