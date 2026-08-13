#include <benchmark/benchmark.h>

#include <vector>

static void BM_vector_push_back(benchmark::State& state) {
    while (state.KeepRunning()) {
        std::vector<int> v;
        v.push_back(3);
    }
}
// Register the loop profile
BENCHMARK(BM_vector_push_back);
