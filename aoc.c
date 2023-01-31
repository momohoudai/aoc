#include "aoc.h"

#define solve(func, input) \
  printf(#func ": ");\
  func(input); \
  printf("\n");


int main() {
  printf("=== AOC 2022 Solutions ===\n");
  solve(d1a, "input_d1");
  solve(d2a, "input_d2");
  solve(d2b, "input_d2");
  solve(d3a, "input_d3");
  solve(d3b, "input_d3");
  solve(d4a, "input_d4");
  solve(d4b, "input_d4");
  solve(d5a, "input_d5");
  solve(d5b, "input_d5");
  solve(d6a, "input_d6");
  solve(d6b, "input_d6");
  solve(d7a, "input_d7");
  solve(d7b, "input_d7");
  solve(d8a, "input_d8");
  solve(d8b, "input_d8");
  solve(d9a, "input_d9");
  solve(d9b, "input_d9");
  solve(d10a, "input_d10");
  solve(d10b, "input_d10");
  solve(d11a, "input_d11");
  //solve(d11b, "input_d11");
  solve(d12a, "input_d12");
  solve(d12b, "input_d12");
}
