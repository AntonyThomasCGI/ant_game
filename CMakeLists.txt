cmake_minimum_required(VERSION 3.21)
project(antgame VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include_directories(
    external/glfw-3.3.8.bin.WIN64/include
    external/include
)
link_directories(
    external/glfw-3.3.8.bin.WIN64/lib-vc2019
)

file(GLOB SOURCES "src/*.cpp" "src/*.c" "src/*.hpp" "src/*.h")

message(STATUS "Adding Source(s) to executable: ${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glfw3)
