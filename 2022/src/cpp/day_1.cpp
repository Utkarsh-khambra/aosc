#include <fmt/core.h>

#include <algorithm>
#include <fstream>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

#include "frwd_decl.hpp"
auto parse_file(std::string_view filename);
auto parse_file(std::string_view filename)
{
    std::fstream input_file(filename.data());
    std::priority_queue<long> total_calories_per_elf;
    std::string line;
    long total_calories = 0;
    while (std::getline(input_file, line))
    {
        if (line.empty())
        {
            total_calories_per_elf.push(total_calories);
            total_calories = 0;
            continue;
        }
        total_calories += std::stoll(line);
    }
    return total_calories_per_elf;
}

void day1() noexcept
{
    std::string home(std::getenv("HOME"));
    auto calories = parse_file(home + "/Dropbox/aosc/2022/src/data/day1_input.txt");
    fmt::print("Max is {}\n", calories.top());
    auto const top_three_sum = [&] {
        auto sum = calories.top();
        calories.pop();
        sum += calories.top();
        calories.pop();
        sum += calories.top();
        calories.pop();
        return sum;
    }();
    fmt::print("Top three is {}\n", top_three_sum);
}
