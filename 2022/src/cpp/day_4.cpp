#include <fmt/core.h>
#include <fmt/ranges.h>

#include <cstdlib>
#include <fstream>
#include <string>

#include "frwd_decl.hpp"

[[nodiscard]] auto parse_pair_range(std::string_view line) noexcept
{
    auto elf_1_range = std::string_view(line.begin(), line.find(','));
    auto elf_2_range =
        std::string_view(line.begin() + elf_1_range.size() + 1, line.size() - elf_1_range.size() - 1);

    auto get_range = [](std::string_view range_str) noexcept {
        auto seperator_pos = range_str.find('-');
        auto range_start = std::stol(std::string(range_str.substr(0, seperator_pos)));
        auto range_end = std::stol(std::string(range_str.substr(seperator_pos + 1)));
        return std::make_pair(range_start, range_end);
    };

    return std::make_pair(get_range(elf_1_range), get_range(elf_2_range));
}

[[nodiscard]] bool has_full_overlap(std::pair<long, long> a, std::pair<long, long> b) noexcept
{
    if (a.first == b.first || a.second == b.second)
        return true;
    if (a.first < b.first)
        return a.second > b.second;
    return a.second < b.second;
}

[[nodiscard]] bool has_partial_overlap(std::pair<long, long> a, std::pair<long, long> b) noexcept
{
    if (a.first == b.first || a.second == b.second)
        return true;
    if (a.first < b.first)
        return a.second >= b.first;
    return a.first <= b.second;
}

void day4() noexcept
{
    std::string line;
    std::string home(std::getenv("HOME"));
    std::ifstream file(home + "/Dropbox/aosc/2022/src/data/day4_input.txt");
    if (!file.is_open())
    {
        fmt::print("Couln't open the file");
        abort();
    }

    long part_1_count = 0;
    long part_2_count = 0;
    while (std::getline(file, line))
    {
        auto [elf_1, elf_2] = parse_pair_range(line);
        part_1_count += has_full_overlap(elf_1, elf_2);
        part_2_count += has_partial_overlap(elf_1, elf_2);
    }
    fmt::print("Part 1 solution {}\n", part_1_count);
    fmt::print("Part 2 solution {}\n", part_2_count);
}
