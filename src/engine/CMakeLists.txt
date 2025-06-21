set(LIBRARY_NAME engine)

find_package(Vulkan REQUIRED)

add_library(${LIBRARY_NAME}
    #resources/resource_manager.cpp
    #resources/resource_manager.hpp
    app.cpp
    app.hpp
    components/material.hpp
    components/mesh.hpp
    components/transform.hpp
    devices/window.cpp
    devices/window.hpp
    engine.cpp
    engine.hpp
    geometry/mesh.cpp
    geometry/mesh.hpp
    geometry/shapes/square.hpp
    geometry/vertex.hpp
    graphics/command_buffer.cpp
    graphics/command_buffer.hpp
    graphics/command_pool.cpp
    graphics/command_pool.hpp
    graphics/constants.hpp
    graphics/context.cpp
    graphics/context.hpp
    graphics/graphics.cpp
    graphics/graphics.hpp
    graphics/instance.cpp
    graphics/instance.hpp
    graphics/logical_device.cpp
    graphics/logical_device.hpp
    graphics/material.cpp
    graphics/material.hpp
    graphics/physical_device.cpp
    graphics/physical_device.hpp
    graphics/pipeline.cpp
    graphics/pipeline.hpp
    graphics/render_pass.cpp
    graphics/render_pass.hpp
    graphics/resource_manager.cpp
    graphics/resource_manager.hpp
    graphics/shader.cpp
    graphics/shader.hpp
    graphics/surface.cpp
    graphics/surface.hpp
    graphics/swap_chain.cpp
    graphics/swap_chain.hpp
    graphics/texture.cpp
    graphics/texture.hpp
    renderers/sprite_renderer.cpp
    renderers/sprite_renderer.hpp
    scenes/game_object.cpp
    scenes/game_object.hpp
    scenes/locator.cpp
    scenes/locator.hpp
)

include_directories(
    external/glfw/include
    external/rocket
)

message(STATUS "vulkan include dir: ${Vulkan_INCLUDE_DIR}")

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Vulkan_INCLUDE_DIR})

target_link_libraries(${LIBRARY_NAME} glfw glm ${GLFW_LIBRARIES} Vulkan::Vulkan)
