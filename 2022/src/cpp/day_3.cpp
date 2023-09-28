#include <fmt/core.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <ranges>
#include <string>

#include "frwd_decl.hpp"

class CharSet
{
    std::uint64_t m_set;
    CharSet(std::uint64_t a): m_set(a) {};

  public:
    CharSet(): m_set(0) {};
    template <std::ranges::range R>
    CharSet(R&& str): m_set(0)
    {
        for (auto i: str)
        {
            m_set = m_set | (i > 'Z' ? 1ULL << (i - 'a') : 1ULL << (i - 'A' + 26));
        }
    }
    [[nodiscard]] bool check(char c) const noexcept
    {
        if (c > 'Z')
        {
            return m_set & (1ULL << (c - 'a'));
        }
        return m_set & (1ULL << (c - 'A' + 26));
    }
    [[nodiscard]] CharSet operator&(CharSet a) noexcept { return a.m_set & this->m_set; }

    explicit operator std::string()
    {

        std::string ret;
        for (auto i: std::views::iota(0, 26))
        {
            if (this->check('A' + i))
                ret.push_back('A' + i);
        }
        for (auto i: std::views::iota(0, 26))
        {
            if (this->check('a' + i))
                ret.push_back('a' + i);
        }
        return ret;
    }
};

[[nodiscard]] CharSet charset_intersection(CharSet a, CharSet b) noexcept
{
    return a & b;
}

[[nodiscard]] int score(char c) noexcept
{
    if (c > 'Z')
    {
        return c - 'a' + 1;
    }
    return c - 'A' + 27;
}

void day3() noexcept
{
    std::string line;
    std::string home(std::getenv("HOME"));
    std::ifstream file(home + "/Dropbox/aosc/2022/src/data/day3_input.txt");
    if (!file.is_open())
    {
        fmt::print("Couldn't open input file\n");
        abort();
    }
    long part1_sum = 0;
    long part2_sum = 0;
    std::array<CharSet, 3> line_group;
    size_t line_index = 0;

    while (std::getline(file, line))
    {
        assert(line.size() % 2 == 0);

        CharSet first_half(line | std::views::take(line.size() / 2));
        CharSet second_half(line | std::views::drop(line.size() / 2));
        line_group[line_index++] = CharSet(line);

        if (line_index > 2)
        {
            auto intersection = charset_intersection(line_group[0], line_group[1]);
            intersection = charset_intersection(intersection, line_group[2]);
            auto i = std::string(intersection).front(); // Only need just one character
            part2_sum += score(i);
            line_index = 0;
        }

        auto i = std::string(charset_intersection(first_half, second_half))
                     .front(); // Only need just one character

        part1_sum += score(i);
    }
    fmt::print("First part solu: {}\n", part1_sum);
    fmt::print("Second part solu: {}\n", part2_sum);
}
