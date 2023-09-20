#include "frwd_decl.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using Pos = std::pair<int, int>;

Pos move_tail(Pos head, Pos tail) noexcept {
  auto [x, y] = head;
  auto [a, b] = tail;
  if (std::fabs(a - x) < 2 && y == b) {
    return tail;
  }
  if (std::fabs(b - y) < 2 && a == x) {
    return tail;
  }
  if (std::fabs(a - x) < 2 && std::fabs(y - b) < 2) {
    return tail;
  }
  if (std::fabs(x - a) == 2 && y == b) {
    int new_x = a + (x > a ? 1 : -1);
    return std::make_pair(new_x, b);
  }
  if (std::fabs(y - b) == 2 && a == x) {
    auto new_y = b + (y > b ? 1 : -1);
    return std::make_pair(a, new_y);
  }
  if (x != a && y != b) {
    auto new_y = b + (y > b ? 1 : -1);
    auto new_x = a + (x > a ? 1 : -1);
    return std::make_pair(new_x, new_y);
  }
  __builtin_unreachable();
}

auto move_left(Pos &head, Pos tail) {
  head.first -= 1;
  return move_tail(head, tail);
}
auto move_right(Pos &head, Pos tail) {
  head.first += 1;
  return move_tail(head, tail);
}
auto move_up(Pos &head, Pos tail) {
  head.second += 1;
  return move_tail(head, tail);
}
auto move_down(Pos &head, Pos tail) {
  head.second -= 1;
  return move_tail(head, tail);
}

void day9() noexcept {
  std::string home(std::getenv("HOME"));
  std::ifstream file(home + "/aoc/src/data/day9_input.txt");
  if (!file.is_open())
    fmt::print("Failed to open input file\n");
  std::stringstream s(R"=(R 4
U 4
L 3
D 1
R 4
D 1
L 5
R 2)=");
  std::string line;
  Pos head(0, 0);
  std::vector<Pos> heads(9, head);
  std::vector<Pos> tail_pos(1, head);
  while (std::getline(file, line)) {
    auto const dist = std::stoi(line.substr(2));
    if (line.starts_with("R")) {
      for (auto i = 0; i < dist; ++i) {
        heads[1] = move_right(heads[0], heads[1]);
        for (auto j = 1u; j < 8u; ++j) {
          heads[j + 1] = move_tail(heads[j], heads[j + 1]);
        }
        tail_pos.push_back(move_tail(heads[8], tail_pos.back()));
      }
    } else if (line.starts_with("L")) {
      for (auto i = 0; i < dist; ++i) {
        heads[1] = move_left(heads[0], heads[1]);
        for (auto j = 1u; j < 8u; ++j) {
          heads[j + 1] = move_tail(heads[j], heads[j + 1]);
        }
        tail_pos.push_back(move_tail(heads[8], tail_pos.back()));
      }
    } else if (line.starts_with("D")) {
      for (auto i = 0; i < dist; ++i) {
        heads[1] = move_down(heads[0], heads[1]);
        for (auto j = 1u; j < 8u; ++j) {
          heads[j + 1] = move_tail(heads[j], heads[j + 1]);
        }
        tail_pos.push_back(move_tail(heads[8], tail_pos.back()));
      }
    } else if (line.starts_with("U")) {
      for (auto i = 0; i < dist; ++i) {
        heads[1] = move_up(heads[0], heads[1]);
        for (auto j = 1u; j < 8u; ++j) {
          heads[j + 1] = move_tail(heads[j], heads[j + 1]);
        }
        tail_pos.push_back(move_tail(heads[8], tail_pos.back()));
      }
    }
  }
  std::ranges::sort(tail_pos);
  auto ret = std::ranges::unique(tail_pos);
  tail_pos.erase(ret.begin(), ret.end());
  fmt::print("{}\n", tail_pos.size());
}
// Remove the code from for loop in each of if to get answer for part 1
