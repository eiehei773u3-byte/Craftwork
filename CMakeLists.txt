cmake_minimum_required(VERSION 3.20)
project(Craftwork VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Wpedantic)

# 1. Поиск установленных системных библиотек SDL2 и SDL2_image
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(include)

add_executable(Craftwork
    src/core/root.cpp
)

# 2. Подключение заголовочных файлов и линковка библиотек к нашему исполняемому файлу
target_include_directories(Craftwork PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(Craftwork PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
