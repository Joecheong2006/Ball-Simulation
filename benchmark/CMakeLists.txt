find_package(benchmark CONFIG REQUIRED)

add_executable(bench
    main.cpp
    BM_DynamicDispatch.cpp
)

target_link_libraries(bench PRIVATE
    SimulationLib
    benchmark::benchmark
)
