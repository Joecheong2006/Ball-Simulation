cmake_minimum_required(VERSION 3.14)
project(BallSimulation LANGUAGES CXX)

if(NOT DEFINED OUTPUT_SUBDIR)
    set(OUTPUT_SUBDIR "bin")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../${OUTPUT_SUBDIR})

if(CMAKE_CONFIGURATION_TYPES)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIG} CONFIG_UPPER)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} ${CMAKE_BINARY_DIR}/../${OUTPUT_SUBDIR})
    endforeach()
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib/glad)
add_subdirectory(src)

add_executable(Simulation src/main.cpp)
target_compile_definitions(Simulation PUBLIC PROJECT_ROOT="${CMAKE_SOURCE_DIR}")
target_link_libraries(Simulation PRIVATE SimulationLib)

option(BUILD_BENCHMARKS "Compile the high-performance benchmark suite" OFF)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()
