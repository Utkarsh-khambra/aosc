#include <fmt/core.h>

#include <bit>
#include <cassert>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

#include "frwd_decl.hpp"

[[nodiscard]] size_t find_first_n_unique_chars(std::string_view message, unsigned int n) noexcept
{
    unsigned int set = 0;
    assert(n <= 26);
    for (size_t i = 0; i < message.size() - n + 1; ++i)
    {
        for (size_t j: std::views::iota(0u, n))
            set = set | (1u << (message[i + j] - 'a'));
        if (std::popcount(set) == static_cast<int>(n))
        {
            return i + n;
            break;
        }
        set = 0;
    }
    __builtin_unreachable();
}

void day6() noexcept
{
    std::string home(std::getenv("HOME"));
    std::ifstream file(home + "/Dropbox/aosc/2022/src/data/day6_input.txt");
    if (!file.is_open())
    {
        fmt::print("Couldn't open the input file\n");
        abort();
    }
    std::string message { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    fmt::print("Part one {}\n", find_first_n_unique_chars(message, 4));
    fmt::print("Second one {}\n", find_first_n_unique_chars(message, 14));
}
