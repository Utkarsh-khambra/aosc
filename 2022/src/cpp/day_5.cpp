#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

void move_part_1(std::vector<std::vector<char>>& stacks, long amount, long from, long to) noexcept
{
    for (auto i: std::views::iota(0, amount))
    {
        stacks[static_cast<size_t>(to - 1)].push_back(stacks[static_cast<size_t>(from - 1)].back());
        stacks[static_cast<size_t>(from - 1)].pop_back();
    }
}

void move_part_2(std::vector<std::vector<char>>& stacks, long amount, long from, long to) noexcept
{
    move_part_1(stacks, amount, from, to);
    auto& current_stack = stacks[static_cast<size_t>(to - 1)];
    std::ranges::reverse(current_stack
                         | std::views::drop(current_stack.size() - static_cast<size_t>(amount)));
}

void day5() noexcept
{

    std::string home(std::getenv("HOME"));
    std::ifstream file(home + "/Dropbox/aosc/2022/src/data/day5_input.txt");
    if (!file.is_open())
    {
        fmt::print("Couldn't open the input file\n");
        abort();
    }
    std::string line;
    std::getline(file, line);
    file.seekg(0, std::ios_base::beg);
    assert((line.size() + 1) % 4 == 0);
    auto const total_stacks = (line.size() + 1) / 4;
    std::vector<std::vector<char>> stacks(total_stacks);
    while (true)
    {
        std::getline(file, line);
        if (line.empty())
            break;
        for (auto i = 0u; i < stacks.size(); ++i)
        {
            if (std::isalpha(line[4 * i + 1]))
                stacks[i].push_back(line[4 * i + 1]);
        }
    }
    std::ranges::for_each(stacks, [](auto& a) noexcept { std::ranges::reverse(a); });

    while (std::getline(file, line))
    {
        std::array<long, 3> params;
        size_t params_index = 0;
        // Parse integers
        for (std::string_view str(line); !str.empty();)
        {

            while (!str.empty() && (std::isalpha(str.front()) || std::isspace(str.front())))
                str.remove_prefix(1);
            size_t count = 0;
            while (!str.empty() && std::isdigit(str[count++]))
                ;
            auto integer = std::string_view(str.begin(), count - 1);
            params[params_index++] = std::stol(std::string(integer));
            str.remove_prefix(integer.size());
        }
        // move_part_1(stacks, params[0], params[1], params[2]);
        move_part_2(stacks, params[0], params[1], params[2]);
    }
    for (auto& i: stacks)
    {
        fmt::print("{}", i.back());
    }
    fmt::print("\n");
}
