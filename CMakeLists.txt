cmake_minimum_required(VERSION 3.14)
project(BallSimulation LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lib/glad)
add_subdirectory(src)

add_executable(Simulation src/main.cpp)
target_compile_definitions(Simulation PUBLIC PROJECT_ROOT="${CMAKE_SOURCE_DIR}")
target_link_libraries(Simulation PRIVATE SimulationLib)

option(BUILD_BENCHMARKS "Compile the high-performance benchmark suite" OFF)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
