#include <fmt/core.h>
#include <fmt/ranges.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <ranges>
#include <string>

#include "frwd_decl.hpp"

void day12() noexcept
{
    std::string line;
    std::string home(std::getenv("HOME"));
    std::ifstream file(home + "/Dropbox/aosc/2022/src/data/day12_input.txt");
    fmt::print("{}", home + "/Dropbox/aosc/2022/src/data/day12_input.txt");
    if (!file.is_open())
    {
        fmt::print("Couldn't open input file\n");
        abort();
    }
    while (std::getline(file, line))
    {
    }
}
