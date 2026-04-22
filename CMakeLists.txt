cmake_minimum_required(VERSION 3.10)
project(hot-key-app)

set(CMAKE_CXX_STANDARD 17)

# Зависимости
find_package(X11 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

# linux-x11
add_executable(linux-x11 linux-x11.cpp)
target_include_directories(linux-x11 PRIVATE ${X11_INCLUDE_DIR})
target_link_libraries(linux-x11 PRIVATE ${X11_LIBRARIES})

# linux-wayland
add_executable(linux-wayland linux-wayland.cpp)
target_include_directories(linux-wayland PRIVATE ${DBUS_INCLUDE_DIRS})
target_link_libraries(linux-wayland PRIVATE ${DBUS_LIBRARIES})

add_executable(linux-wayland.chat linux-wayland.chat.cpp)
target_include_directories(linux-wayland.chat PRIVATE ${DBUS_INCLUDE_DIRS})
target_link_libraries(linux-wayland.chat PRIVATE ${DBUS_LIBRARIES})