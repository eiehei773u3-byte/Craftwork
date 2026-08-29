cmake_minimum_required(VERSION 3.20)
project(Craftwork LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем компилятору искать заголовочные файлы в локальной папке include
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Craftwork
    src/core/root.cpp
)

# Линкуем скомпилированные бинарники библиотек из системы
target_link_libraries(Craftwork PRIVATE
    SDL2
    SDL2_image
)
