#include <algorithm>
#include <cassert>
#include <charconv>
#include <fstream>
#include <iostream>
#include <limits>
#include <ranges>
#include <vector>
int main()
{
    using std::operator""sv;
    std::ifstream input("../input/day1");
    assert(input);

    std::vector<int> nums(std::istream_iterator<int> { input }, std::istream_iterator<int>());
    int count = std::ranges::count_if(nums | std::views::adjacent<2>, [](auto a) noexcept {
        // std::cout << std::get<0>(a) << " " << std::get<1>(a) << std::endl;
        return std::get<0>(a) < std::get<1>(a);
    });

    std::cout << count << "\n";

    auto f = nums | std::views::slide(3) | std::views::transform([](auto a) noexcept -> int {
                 int sum = 0;
                 for (auto i: a)
                     sum += i;
                 return sum;
             });

    count = std::ranges::count_if(f | std::views::adjacent<2>, [](auto a) noexcept {
        // std::cout << std::get<0>(a) << " " << std::get<1>(a) << std::endl;
        return std::get<0>(a) < std::get<1>(a);
    });
    ;
    std::cout << count << "\n";
}
