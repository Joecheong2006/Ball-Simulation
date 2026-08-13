find_package(benchmark CONFIG REQUIRED)

add_executable(bench bench.cpp)

target_link_libraries(bench PRIVATE
    SimulationLib
    benchmark::benchmark_main
)
