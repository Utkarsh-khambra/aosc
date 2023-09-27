#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>

template <typename R>
inline int convert_to_int(R&& r)
{
    int temp;
    std::from_chars(r.data(), r.data() + r.size(), temp);
    return temp;
}

int main()
{
    using std::operator""sv;
    std::ifstream input("../input/day2");
    std::string line;
    int total_depth = 0;
    int total_forward = 0;
    int aim = 0;
    int part_2_depth = 0;
    while (std::getline(input, line))
    {
        auto rng = line | std::views::split(" "sv) | std::views::adjacent<2>;
        for (auto const& i: rng)
        {
            if (*std::get<0>(i).begin() == 'f')
            {
                int temp = convert_to_int(std::get<1>(i));
                total_forward += temp;
                part_2_depth += aim * temp;
                continue;
            }
            if (*std::get<0>(i).begin() == 'u')
            {
                aim -= convert_to_int(std::get<1>(i));
                total_depth -= convert_to_int(std::get<1>(i));
                continue;
            }
            if (*std::get<0>(i).begin() == 'd')
            {
                aim += convert_to_int(std::get<1>(i));
                total_depth += convert_to_int(std::get<1>(i));
                continue;
            }
        }
    }
    std::cout << total_forward * total_depth << "\n";
    std::cout << total_forward * part_2_depth << "\n";
}
