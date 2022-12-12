#include "frwd_decl.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <fstream>
#include <functional>
#include <ranges>
#include <string>
#include <unordered_map>

uint64_t shared = 1;

class Monkey {
public:
  void add_item(std::uint64_t item) { item_worry.push_back(item); }
  auto throw_items() {
    if (item_worry.empty())
      return;
    for (auto i : item_worry) {
      // i = std::floor(old_to_new(i) / 3.0f);
      i = old_to_new(i) % shared;
      if (test(i))
        on_true->add_item(i);
      else
        on_false->add_item(i);
    }
    item_worry.clear();
  }

  void add_on_false_monkey(Monkey *m) noexcept { on_false = m; }
  void add_on_true_monkey(Monkey *m) noexcept { on_true = m; }
  void set_old_to_new(std::function<std::uint64_t(std::uint64_t)> f) noexcept {
    old_to_new.swap(f);
  }

  void set_on_true(Monkey *m) noexcept { on_true = m; }
  void set_on_false(Monkey *m) noexcept { on_false = m; }
  void set_divisor(int d) noexcept { div = d; }

  void print() {
    fmt::print("{} {} {} Div {}\n", item_worry, on_false->item_worry,
               on_true->item_worry, div);
  }

  auto held_items() const noexcept { return item_worry.size(); }

private:
  bool test(std::uint64_t w) noexcept { return w % div == 0; }
  std::vector<std::uint64_t> item_worry;
  Monkey *on_false;
  Monkey *on_true;
  std::function<std::uint64_t(std::uint64_t)> old_to_new;
  int div = 0;
};

void day11() noexcept {
  std::string line;
  std::string home(std::getenv("HOME"));
  std::ifstream file(home + "/aoc/src/data/day11_input.txt");
  std::unordered_map<int, Monkey> monkeys;
  if (!file.is_open()) {
    fmt::print("Couldn't open input file\n");
    abort();
  }
  while (std::getline(file, line)) {
    if (line.starts_with("Monkey")) {
      int monkey_id =
          std::stoi(line.substr(line.find(" ") + 1, line.find(":") - 1));
      if (!monkeys.contains(monkey_id))
        monkeys.insert({monkey_id, Monkey()});
      std::getline(file, line);
      auto list = line | std::views::drop(17) | std::views::split(',');
      for (auto i : list) {
        auto item = std::stoi(std::string(i.begin(), i.end()));
        monkeys[monkey_id].add_item(item);
      }
      std::getline(file, line);
      if (line.contains("+")) {
        int constant = std::stoi(line.substr(line.find("+") + 1));
        monkeys[monkey_id].set_old_to_new(
            [=](std::uint64_t w) noexcept -> std::uint64_t {
              return w + static_cast<std::uint64_t>(constant);
            });
      } else {
        try {
          int constant = std::stoi(line.substr(line.find("*") + 1));
          monkeys[monkey_id].set_old_to_new(
              [=](std::uint64_t w) noexcept -> std::uint64_t {
                return w * static_cast<std::uint64_t>(constant);
              });
        } catch (...) {
          monkeys[monkey_id].set_old_to_new(
              [](std::uint64_t w) noexcept -> std::uint64_t { return w * w; });
        }
      }

      std::getline(file, line);
      auto const t = std::stoi(line.substr(line.find("by") + 3));
      shared *= t;
      monkeys[monkey_id].set_divisor(t);

      std::getline(file, line);
      if (line.contains("true")) {
        auto const throw_to = std::stoi(line.substr(line.find("ey") + 3));
        if (!monkeys.contains(throw_to))
          monkeys.insert({throw_to, Monkey()});
        monkeys[monkey_id].set_on_true(&monkeys[throw_to]);
      }
      std::getline(file, line);
      if (line.contains("false")) {
        auto const throw_to = std::stoi(line.substr(line.find("ey") + 3));
        if (!monkeys.contains(throw_to))
          monkeys.insert({throw_to, Monkey()});
        monkeys[monkey_id].set_on_false(&monkeys[throw_to]);
      }
    }
  }
  // for (auto i : monkeys) {
  //   fmt::print("Monkey {}\n", i.first);
  //   i.second.print();
  // }
  std::vector<std::uint64_t> counts(monkeys.size(), 0);
  int total_rounds = 10000;
  for (auto i : std::views::iota(0, total_rounds)) {
    for (auto j : std::views::iota(0, static_cast<int>(monkeys.size()))) {
      counts[j] += monkeys[j].held_items();
      monkeys[j].throw_items();
    }
  }
  std::ranges::sort(counts);
  // fmt::print("Part 1 :{}\n",
  //            counts[counts.size() - 1] * counts[counts.size() - 2]);
  fmt::print("Part 2 : {}\n",
             counts[counts.size() - 1] * counts[counts.size() - 2]);
}
