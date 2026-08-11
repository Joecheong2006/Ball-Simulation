cmake_minimum_required(VERSION 3.14)
project(BallSimulation LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
    GIT_SHALLOW    TRUE
)

# Turn off heavy extra features we don't need to compile
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Make GLFW source code available to the build system
FetchContent_MakeAvailable(glfw)

add_subdirectory(lib/glad)

add_executable(Simulation
    src/main.cpp
    src/Resources.cpp

    src/gl/Buffer.cpp
    src/gl/Shader.cpp
    src/gl/ShaderProgram.cpp

    lib/stb_image/stb_image.cpp
)

target_compile_definitions(Simulation PRIVATE PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

target_include_directories(Simulation PRIVATE include/ lib/)

target_link_libraries(Simulation PRIVATE
    glad
    glfw
)
