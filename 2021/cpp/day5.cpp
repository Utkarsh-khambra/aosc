#include <array>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <vector>

using std::operator""sv;

using Range = std::array<std::pair<unsigned int, unsigned int>, 2>;

int main()
{
    std::ifstream input("../input/day5");
    std::string line;
    std::vector<Range> ranges;
    auto hash = [](std::pair<uint32_t, uint32_t> const& a) -> size_t {
        return ((a.first + a.second) * (a.first + a.second + 1) / 2) + a.second;
    };
    std::unordered_map<std::pair<unsigned int, unsigned int>, int, decltype(hash)> map;
    while (std::getline(input, line))
    {
        ranges.emplace_back();
        for (auto [ind, i]: line | std::views::split("->"sv) | std::views::enumerate)
        {
            auto [start, end] = (i | std::views::split(","sv) | std::views::adjacent<2>) .front();
            std::from_chars(start.data() + ind /*To get rid of the space present just before the end range*/,
                            start.data() + start.size(),
                            ranges.back()[ind].first);
            std::from_chars(end.data(), end.data() + end.size(), ranges.back()[ind].second);
        }
        auto const& [first, last] = ranges.back();
        if (first.first == last.first)
        {
            auto start = first.second;
            auto end = last.second;
            if (start > end)
                std::swap(start, end);
            ++end;
            for (auto i: std::views::iota(start, end))
            {
                ++map[{ first.first, i }];
            }
            ranges.pop_back();
        }
        else if (last.second == first.second)
        {
            auto start = first.first;
            auto end = last.first;
            if (start > end)
                std::swap(start, end);
            ++end;
            for (auto i: std::views::iota(start, end))
            {
                ++map[{ i, first.second }];
            }
            // Don't need to consider these later, so no need to store them.
            ranges.pop_back();
        }
    }
    int count = 0;
    for (auto i: map | std::views::values)
    {
        if (i >= 2)
            ++count;
    }
    std::cout << "Part 1 " << count << "\n";
    for (auto [first, last]: ranges)
    {
        if (first.second > last.second)
        {
            if (first.first < last.first)
            {
                for (; first.second >= last.second;)
                {
                    ++map[{ first.first++, first.second-- }];
                }
            }
            else
            {
                for (; first.second >= last.second;)
                {
                    ++map[{ first.first--, first.second-- }];
                }
            }
        }
        else
        {
            if (first.first < last.first)
            {
                for (; first.second <= last.second;)
                {
                    ++map[{ first.first++, first.second++ }];
                }
            }
            else
            {
                for (; first.second <= last.second;)
                {
                    ++map[{ first.first--, first.second++ }];
                }
            }
        }
    }
    count = 0;
    for (auto i: map | std::views::values)
    {
        if (i >= 2)
            ++count;
    }
    std::cout << "Part 2 " << count << "\n";
}
