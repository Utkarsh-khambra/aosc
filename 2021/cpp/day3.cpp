#include <bit>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>
int main()
{
    std::ifstream input("../input/day3");
    std::string line;
    std::getline(input, line);
    std::vector<int> count;
    int pc = line.size();
    std::vector<unsigned int> vals { 0 };
    for (auto [i, c]: line | std::views::enumerate)
    {
        if (c == '0')
        {
            count.push_back(1);
        }
        else
        {
            count.push_back(0);
        }
        vals[0] |= (c - '0') << (pc - i - 1);
    }
    while (std::getline(input, line))
    {
        unsigned int temp = 0;
        for (auto [i, bit]:
             line | std::views::transform([](auto a) noexcept { return a - '0'; }) | std::views::enumerate)
        {
            count[i] += bit == 0 ? 1 : -1;
            temp |= bit << (pc - i - 1);
        }
        vals.push_back(temp);
    }
    unsigned int gamma = 0;
    unsigned int mask = 0;
    for (auto [i, zero_count]: count | std::views::reverse | std::views::enumerate)
    {
        if (zero_count < 0)
            gamma |= (1U << i);
        mask |= 1U << i;
    }
    std::cout << gamma * (~gamma & mask) << "\n";

    // Part 2 process
    std::vector<unsigned int> copys(vals);
    std::vector<unsigned int> copys2(vals);
    auto* ref = &vals;
    auto* ref2 = &copys;
    for (auto i: std::views::iota(0, pc) | std::views::reverse)
    {
        unsigned int mask = 1U << i;
        int rel_count = 0;
        {
            for (auto j: *ref)
            {
                rel_count += (mask & j) >> i == 0 ? -1 : 1;
            }
            unsigned int filter_bit = rel_count >= 0;
            ref2->clear();
            for (auto j: *ref | std::views::filter([filter_bit, mask, i](auto a) noexcept {
                     return (a & mask) >> i == filter_bit;
                 }))
            {
                ref2->push_back(j);
            }
            if (ref2->size() == 1)
            {
                std::cout << ref2->front() << '\n';
                break;
            }
        }
        auto* t = ref;
        ref = ref2;
        ref2 = t;
    }
    vals = copys2;
    ref = &vals;
    ref2 = &copys2;

    for (auto i: std::views::iota(0, pc) | std::views::reverse)
    {
        unsigned int mask = 1U << i;
        int rel_count = 0;
        {
            for (auto j: *ref)
            {
                rel_count += (mask & j) >> i == 0 ? -1 : 1;
            }
            unsigned int filter_bit = !(rel_count >= 0);
            ref2->clear();
            for (auto j: *ref | std::views::filter([filter_bit, mask, i](auto a) noexcept {
                     return (a & mask) >> i == filter_bit;
                 }))
            {
                ref2->push_back(j);
            }
            if (ref2->size() == 1)
            {
                std::cout << ref2->front() << '\n';
                break;
            }
        }
        auto* t = ref;
        ref = ref2;
        ref2 = t;
    }
}
