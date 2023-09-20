#include "frwd_decl.hpp"
#include <cassert>
#include <concepts>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <span>
#include <string>
#include <vector>

void get_tree_heights_in_row(std::vector<int> &map,
                             std::string_view row_str) noexcept {
  for (auto i : row_str)
    map.push_back(i - '0');
}

template <bool left = true>
size_t find_bigger_tree_in_row(const std::vector<int> &map, size_t current_row,
                               size_t current_col, size_t row_size) {
  auto elem = [&](auto row, auto col) { return map[row * row_size + col]; };
  auto original_col = current_col;
  auto current = [&] {
    if constexpr (left)
      return elem(current_row, current_col--);
    else
      return elem(current_row, current_col++);
  }();
  if constexpr (left) {
    for (; current_col >= 0 && current_col < static_cast<size_t>(-1);
         --current_col) {
      if (elem(current_row, current_col) >= current)
        return current_col;
    }
  } else {
    for (; current_col < row_size; ++current_col) {
      if (elem(current_row, current_col) >= current)
        return current_col;
    }
  }
  return original_col;
}

template <bool up = true>
size_t find_bigger_tree_in_col(const std::vector<int> &map, size_t current_row,
                               size_t current_col, size_t row_size) {
  auto elem = [&](auto row, auto col) { return map[row * row_size + col]; };
  auto original_row = current_row;
  auto total_rows = map.size() / row_size;
  auto const current = [&] {
    if constexpr (up)
      return elem(current_row--, current_col);
    else
      return elem(current_row++, current_col);
  }();
  if constexpr (up) {
    for (; current_row >= 0 && current_row < static_cast<size_t>(-1);
         --current_row) {
      if (elem(current_row, current_col) >= current) {
        return current_row;
      }
    }
  } else {
    for (; current_row < total_rows; ++current_row) {
      if (elem(current_row, current_col) >= current)
        return current_row;
    }
  }
  return original_row;
}

int find_visible_trees(std::vector<int> &map, size_t row_size) {
  auto elem = [&](auto row, auto col) { return map[row * row_size + col]; };
  size_t total_rows = map.size() / row_size;
  int count = 0;

  auto is_greatest_left = [&](auto row, auto col) {
    return col == find_bigger_tree_in_row<true>(map, row, col, row_size);
  };
  auto is_greatest_right = [&](auto row, auto col) {
    return col == find_bigger_tree_in_row<false>(map, row, col, row_size);
  };
  auto is_greatest_up = [&](auto row, auto col) {
    return row == find_bigger_tree_in_col<true>(map, row, col, row_size);
  };
  auto is_greatest_down = [&](auto row, auto col) {
    return row == find_bigger_tree_in_col<false>(map, row, col, row_size);
  };
  for (auto i = 0; i < row_size; ++i)
    fmt::print("\033[48;5;196m{}\033[0m", elem(0, i));
  fmt::print("\n");
  for (auto row = 1u; row < total_rows - 1; ++row) {
    fmt::print("\033[48;5;196m{}\033[0m", elem(row, 0));
    for (auto col = 1u; col < row_size - 1; ++col) {
      if (is_greatest_left(row, col) || is_greatest_right(row, col) ||
          is_greatest_down(row, col) || is_greatest_up(row, col)) {
        fmt::print("\033[48;5;196m{}\033[0m", elem(row, col));
        ++count;
      } else {
        fmt::print("{}", elem(row, col));
      }
    }
    fmt::print("\033[48;5;196m{}\033[0m\n", elem(row, row_size - 1));
  }
  for (auto i = 0; i < row_size; ++i)
    fmt::print("\033[48;5;196m{}\033[0m", elem(total_rows - 1, i));
  fmt::print("\n");
  return count + static_cast<int>((2 * (row_size + total_rows))) - 4;
}

size_t find_highest_score(const std::vector<int> &map, size_t row_size) {
  auto find_bigger_left = [&](auto row, auto col) {
    return find_bigger_tree_in_row<true>(map, row, col, row_size);
  };
  auto find_bigger_right = [&](auto row, auto col) {
    return find_bigger_tree_in_row<false>(map, row, col, row_size);
  };
  auto find_bigger_up = [&](auto row, auto col) {
    return find_bigger_tree_in_col<true>(map, row, col, row_size);
  };
  auto find_bigger_down = [&](auto row, auto col) {
    return find_bigger_tree_in_col<false>(map, row, col, row_size);
  };
  auto const total_rows = map.size() / row_size;
  size_t max = 0;
  for (auto i = 0; i < total_rows; ++i) {
    for (auto j = 0; j < row_size; ++j) {
      size_t score = 0;
      if (auto k = find_bigger_left(i, j); k != j) {
        score = j - k;
      } else
        score = j;

      if (auto k = find_bigger_right(i, j); k != j) {
        score *= k - j;
      } else
        score *= (row_size - 1 - j);
      if (auto k = find_bigger_up(i, j); k != i) {
        score *= i - k;
      } else
        score *= i;
      if (auto k = find_bigger_down(i, j); k != i) {
        score *= k - i;
      } else
        score *= total_rows - 1 - i;
      if (score > max)
        max = score;
    }
  }
  return max;
}

void day8() noexcept {
  std::string home(std::getenv("HOME"));
  std::ifstream file(home + "/aoc/src/data/day8_input.txt");
  if (!file.is_open()) {
    fmt::print("Couldn't open the input file\n");
    abort();
  }
  std::string line;
  std::getline(file, line);
  size_t row_size = line.size();
  std::vector<int> map;
  get_tree_heights_in_row(map, line);
  while (std::getline(file, line)) {
    get_tree_heights_in_row(map, line);
  }
  fmt::print("First part sol: {}\n", find_visible_trees(map, row_size));
  fmt::print("Second part sol: {}\n", find_highest_score(map, row_size));
}
