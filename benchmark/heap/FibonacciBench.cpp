// Standard headers
#include <cmath>
#include <chrono>
#include <random>
#include <vector>
#include <iostream>

// External headers
#include "benchmark/benchmark.h"

// Internal headers
#include "graph/Graph.hpp"
#include "graph/dijkstra.hpp"

// Benchmarked header
#include "heap/Fibonacci.hpp"

/*============================================================================*/

static void BM_DijkstraMinimumPathWithFibonacciHeap(benchmark::State& state) {
  auto num_nodes = state.range_x();
  auto num_edges = 2*num_nodes;
  auto max_weight = 1000.0;

  unsigned int i = 0;
  while (state.KeepRunning()) {
    // state.PauseTiming();
    auto graph = graph::generateRandomGraph(num_nodes,
                                            num_edges,
                                            max_weight,
                                            std::mt19937{i++});
    // state.ResumeTiming();

    auto start = std::chrono::high_resolution_clock::now();
    auto path = graph::dijkstra<heap::Fibonacci>(graph, 0, num_nodes-1);
    auto end   = std::chrono::high_resolution_clock::now();

    auto elapsed_seconds =
      std::chrono::duration_cast<std::chrono::duration<double>>(
        end - start);

    state.SetIterationTime(elapsed_seconds.count());
  }
}

/*----------------------------------------------------------------------------*/

BENCHMARK(BM_DijkstraMinimumPathWithFibonacciHeap)
  ->RangeMultiplier(2)->Range(512, 4*1024*1024)->UseManualTime();

/*============================================================================*/
