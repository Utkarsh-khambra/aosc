#include "frwd_decl.hpp"
#include <algorithm>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

int signal_strength(int v, int cycle) noexcept { return v * cycle; }

void day10() noexcept {
  std::string home(std::getenv("HOME"));
  std::ifstream file(home + "/aoc/src/data/day10_input.txt");
  if (!file.is_open())
    fmt::print("Failed to open input file\n");
  std::string line;
  int register_x = 1;
  int cycle = 0;
  int sum = 0;
  std::stringstream s(R"=(addx 15
addx -11
addx 6
addx -3
addx 5
addx -1
addx -8
addx 13
addx 4
noop
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx 5
addx -1
addx -35
addx 1
addx 24
addx -19
addx 1
addx 16
addx -11
noop
noop
addx 21
addx -15
noop
noop
addx -3
addx 9
addx 1
addx -3
addx 8
addx 1
addx 5
noop
noop
noop
noop
noop
addx -36
noop
addx 1
addx 7
noop
noop
noop
addx 2
addx 6
noop
noop
noop
noop
noop
addx 1
noop
noop
addx 7
addx 1
noop
addx -13
addx 13
addx 7
noop
addx 1
addx -33
noop
noop
noop
addx 2
noop
noop
noop
addx 8
noop
addx -1
addx 2
addx 1
noop
addx 17
addx -9
addx 1
addx 1
addx -3
addx 11
noop
noop
addx 1
noop
addx 1
noop
noop
addx -13
addx -19
addx 1
addx 3
addx 26
addx -30
addx 12
addx -1
addx 3
addx 1
noop
noop
noop
addx -9
addx 18
addx 1
addx 2
noop
noop
addx 9
noop
noop
noop
addx -1
addx 2
addx -37
addx 1
addx 3
noop
addx 15
addx -21
addx 22
addx -6
addx 1
noop
addx 2
addx 1
noop
addx -10
noop
noop
addx 20
addx 1
addx 2
addx 2
addx -6
addx -11
noop
noop
noop)=");
  std::unordered_set<int> interesting_cycles{20, 60, 100, 140, 180, 220};
  while (std::getline(file, line)) {
    if (line.starts_with("ad")) {
      int v = std::stoi(line.substr(line.find(" ") + 1));
      for (auto i : std::views::iota(0, 2)) {
        ++cycle;
        if (interesting_cycles.contains(cycle)) {
          sum += register_x * cycle;
          fmt::print("{} {} {}\n", register_x, v, cycle);
        }
      }
      register_x += v;
    }
    if (line.starts_with("no")) {
      ++cycle;
      if (interesting_cycles.contains(cycle)) {
        fmt::print("{} {} \n", register_x, cycle);
        sum += register_x * cycle;
      }
    }
  }
  fmt::print("First part sol {}\n", sum);
}
