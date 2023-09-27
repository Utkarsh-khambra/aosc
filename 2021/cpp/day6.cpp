#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>
#include <cassert>
int main()
{
    std::fstream input("input/day6");
    std::string line;
    std::vector<int> fish;
    while (std::getline(input, line, ','))
    {
        assert(!line.empty());
        if (!line.ends_with('\n'))
        {
            fish.push_back(std::stoi(line));
        }
        else
        {
            fish.push_back(std::stoi(line.substr(0, line.size() - 1)));
        }
    }
}
