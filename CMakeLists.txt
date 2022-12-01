cmake_minimum_required(VERSION 3.21)
project(homework)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
add_subdirectory(external)

add_executable(homework
    src/main.cpp
    src/rendering/GLWindow.cpp
    src/rendering/Texture.cpp
    src/app/Time.cpp
    src/app/Camera.cpp
    src/rendering/ShaderProgram.cpp
    src/rendering/Shader.cpp
    src/app/Gui.cpp
    src/app/BasicApplication.cpp
    
    src/game/MeshRenderer.cpp
    src/game/Mesh.cpp
    src/game/Player.cpp src/game/Menu.cpp src/game/Menu.h)

target_include_directories(homework PUBLIC src)
target_compile_definitions(homework PRIVATE GLFW_INCLUDE_NONE)
target_compile_options(homework PRIVATE $<$<CXX_COMPILER_ID:MSVC>: /W4 /WX> $<$<NOT:$<CXX_COMPILER_ID:MSVC>>: -Wall -Wextra -Wpedantic>)
target_link_libraries(homework PRIVATE OpenGL::GL OpenGL::GLU glfw glad assimp stb_image imgui tiny_obj_loader
    $<$<PLATFORM_ID:Windows>:glm> $<$<PLATFORM_ID:Linux>:glm::glm>)
