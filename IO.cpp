#include <cmath>
#include <cstdlib>
#include <iostream>
#include "get_time.h"
#include "parallel.h"

using Type = long long;

int main(int argc, char* argv[]) {
  size_t n = 1e9;
  int num_rounds = 3;
  if (argc >= 2) {
    n = atoll(argv[1]);
  }
  if (argc >= 3) {
    num_rounds = atoi(argv[2]);
  }

  Type* A = (Type*)malloc(n * sizeof(Type));
  parlay::parallel_for(0, n, [&](size_t i) { A[i] = i; });

  double total_time_1 = 0;
  for (int i = 0; i < num_rounds; i++) {
    parlay::timer t;
    parlay::parallel_for(0, n, [&](size_t i) { A[i] *= 10; });
    t.stop();

    std::cout << "Round " << i << " function 1 running time: " << t.total_time() << std::endl;
    total_time_1 += t.total_time();
  }

  double total_time_2 = 0;
  for (int i = 0; i < num_rounds; i++) {
    parlay::timer t;
    parlay::parallel_for(0, n, [&](size_t i) { A[(( long long )i *93) %n] *= 10; });
    t.stop();

    std::cout << "Round " << i << " function 2 running time: " << t.total_time() << std::endl;
    total_time_2 += t.total_time();
  }

  std::cout << "Average running time for function 1: " << total_time_1 / num_rounds << std::endl;
  std::cout << "Average running time for function 2: " << total_time_2 / num_rounds << std::endl;

  return 0;
}
