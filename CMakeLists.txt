cmake_minimum_required(VERSION 3.16)
project(pong-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(pong src/main.cpp src/Game.cpp src/Paddle.cpp src/Ball.cpp)

target_include_directories(pong PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(pong PRIVATE ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)
