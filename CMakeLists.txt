cmake_minimum_required(VERSION 3.20)
project(Craftwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(GLEW REQUIRED glew)

add_executable(Craftwork
    src/Core/Root.cpp
    src/Core/main.cpp
)

target_include_directories(Craftwork PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(Craftwork PRIVATE
    ${SDL2_LIBRARIES}
    ${GLEW_LIBRARIES}
    GL
)
