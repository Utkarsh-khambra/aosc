#include "frwd_decl.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <ranges>
#include <string>

void day12() noexcept {
  std::string line;
  std::string home(std::getenv("HOME"));
  std::ifstream file(home + "/aoc/src/data/day12_input.txt");
  if (!file.is_open()) {
    fmt::print("Couldn't open input file\n");
    abort();
  }
  while (std::getline(file, line)) {
  }
}
