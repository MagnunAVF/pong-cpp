cmake_minimum_required(VERSION 3.16)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" PONG_VERSION_RAW)
string(STRIP "${PONG_VERSION_RAW}" PONG_VERSION)

project(pong-cpp LANGUAGES CXX VERSION ${PONG_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf QUIET)

add_executable(pong src/main.cpp src/Game.cpp src/Paddle.cpp src/Ball.cpp)

target_compile_definitions(pong PRIVATE
    SDL_MAIN_HANDLED
    PONG_VERSION_STRING="${PONG_VERSION}"
)

if(TARGET SDL2::SDL2)
    target_link_libraries(pong PRIVATE SDL2::SDL2)
else()
    target_include_directories(pong PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(pong PRIVATE ${SDL2_LIBRARIES})
endif()

if(TARGET SDL2_ttf::SDL2_ttf)
    target_link_libraries(pong PRIVATE SDL2_ttf::SDL2_ttf)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)
    target_link_libraries(pong PRIVATE PkgConfig::SDL2_TTF)
endif()
