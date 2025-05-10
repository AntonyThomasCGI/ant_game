cmake_minimum_required(VERSION 3.21)
project(vulkan_grid VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


find_package(Vulkan REQUIRED)

add_subdirectory(external/glfw)
add_subdirectory(external/rocket)
add_subdirectory(external/glm)

include_directories(
    external/VulkanmemoryAllocator/include
    external/glfw/include
    external/glm
    external/rocket
    external/stb
)


add_subdirectory(src/engine)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} engine glfw glm ${GLFW_LIBRARIES} ${Vulkan_LIBRARY_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC engine ${Vulkan_INCLUDE_DIR})


# Copy resources folder to build location.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources/)

