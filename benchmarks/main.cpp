#include <adder/adder.hpp>
#include <benchmark/benchmark.h>

void AddingBenchmark(benchmark::State& state) {
  while (state.KeepRunning()) {
    benchmark::DoNotOptimize(
        adder::Add(21, 13)
        );
  }
}

BENCHMARK(AddingBenchmark);

BENCHMARK_MAIN();