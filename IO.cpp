#include <cmath>
#include <cstdlib>
#include <iostream>
#include "get_time.h"
#include "parallel.h"

using Type = long long;

int main(int argc, char* argv[]) {
  size_t n = 1e9;
  size_t k = 5;
  size_t m = 1e4;
  int num_rounds = 3;
  if (argc >= 2) {
    n = atoll(argv[1]);
  }
  if (argc >= 3) {
    num_rounds = atoi(argv[2]);
  }

  Type* A = (Type*)malloc(n * sizeof(Type));
  parlay::parallel_for(0, n, [&](size_t i) { A[i] = i; });
  Type* B = (Type*)malloc(n * sizeof(Type));
  parlay::parallel_for(0, n, [&](size_t i) { B[i] = i; });

  double total_time_1 = 0;
  for (int round = 0; round < num_rounds; round++) {
    parlay::timer t;
    parlay::parallel_for(0, (long long) n * k, [&](size_t i) { A[( long long ) i%n] ++; });
    t.stop();

    std::cout << "Round " << round << " function 1 running time: " << t.total_time() << std::endl;
    total_time_1 += t.total_time();
  }

  double total_time_2 = 0;
  for (int round = 0; round < num_rounds; round++) {
    parlay::timer t;
    parlay::parallel_for(0, n/m, [&](size_t i) { 
      for (size_t l = 0; l < k; l++){
        for (size_t j = i * m; j < (i + 1) * m; j++){
          B[j]++;
        }
      }
    });
    t.stop();

    std::cout << "Round " << round << " function 2 running time: " << t.total_time() << std::endl;
    total_time_2 += t.total_time();
  }

  std::cout << "Average running time for function 1: " << total_time_1 / num_rounds << std::endl;
  std::cout << "Average running time for function 2: " << total_time_2 / num_rounds << std::endl;

  return 0;
}
