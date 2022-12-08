#include "frwd_decl.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

struct fs_tree {
  using children_t = std::vector<std::unique_ptr<fs_tree>>;
  fs_tree(std::string name) : dir_name(name) {}
  std::string dir_name;
  size_t total_file_size = 0;
  fs_tree *parent = nullptr;
  children_t children_dir;
};

size_t count(fs_tree *root) noexcept {
  if (root->children_dir.empty())
    return root->total_file_size;
  size_t child_dir_file_size = 0;
  for (auto &i : root->children_dir) {
    child_dir_file_size += count(i.get());
  }
  root->total_file_size += child_dir_file_size;
  return root->total_file_size;
}

size_t traverse_again(fs_tree *root) noexcept {
  if (root->children_dir.empty()) {
    if (root->total_file_size < 100000)
      return root->total_file_size;
    return 0;
  }
  size_t total = 0;
  for (auto &i : root->children_dir) {
    total += traverse_again(i.get());
  }
  if (root->total_file_size < 100000)
    return total + root->total_file_size;
  return total;
}

size_t find_smallest_largest_number(fs_tree *root, size_t n) noexcept {
  if (root->children_dir.empty())
    return root->total_file_size;
  // size_t max = std::numeric_limits<size_t>::max();
  std::vector<size_t> maxs;
  for (auto &i : root->children_dir) {
    auto temp = find_smallest_largest_number(i.get(), n);
    if (temp > n)
      maxs.push_back(temp);
  }
  auto itr = std::ranges::min_element(maxs);
  return itr == maxs.end() ? root->total_file_size : *itr;
}

void day7() noexcept {
  std::string home(std::getenv("HOME"));
  std::ifstream file(home + "/aoc/src/data/day7_input.txt");
  if (!file.is_open()) {
    fmt::print("Couldn't open the input file\n");
    abort();
  }
  std::string line;
  std::unique_ptr<fs_tree> root = std::make_unique<fs_tree>("/");
  fs_tree *current = root.get();
  while (std::getline(file, line)) {
    if (line.starts_with("$")) {
      if (line.substr(2).starts_with("cd")) {
        auto change_dir_name = line.substr(5);
        if (change_dir_name == "/")
          continue;
        if (change_dir_name == "..") {
          current = current->parent;
          continue;
        }
        auto itr = std::ranges::find(current->children_dir, change_dir_name,
                                     &fs_tree::dir_name);
        if (itr == current->children_dir.end()) {
          fmt::print("The fuck\n");
          abort();
        }
        current = itr->get();
      } else if (line.substr(2).starts_with("ls")) {
      }
    } else if (line.starts_with("dir")) {
      current->children_dir.emplace_back(
          std::make_unique<fs_tree>(line.substr(4)));
      current->children_dir.back()->parent = current;
    } else {
      current->total_file_size += std::stoull(line.substr(0, line.find(" ")));
    }
  }
  count(root.get());
  fmt::print("First part sol: {}\n", traverse_again(root.get()));
  size_t space_to_aquaire = 30000000U - (70000000U - root->total_file_size);
  fmt::print("Second part sol: {}\n",
             find_smallest_largest_number(root.get(), space_to_aquaire));
}
