#include <algorithm>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

namespace v = std::views;
using std::operator""sv;

constexpr uint64_t mask = 0x1F;

class BingoBoard
{
  public:
    // Returns true if this mark will cause a bingo
    bool mark(int call) noexcept
    {
        if (board_values.contains(call) && !won)
        {
            auto [row, col] = board_values[call];
            won =  set_row_element(row, col) || set_col_element(row, col);
            return won;
        }
        return false;
    }

    void add_row(std::span<int> row, uint8_t which_row) noexcept
    {
        for (auto [i, val]: row | std::views::enumerate)
        {
            auto res = board_values.try_emplace(val, std::make_pair(which_row, uint8_t { i }));
            assert(res.second);
        }
    }

    auto sum_unmarked() const noexcept
    {
        uint32_t sum = 0;
        for (auto const& [val, pos]: board_values)
        {
            if (!is_marked(pos.first, pos.second))
            {
                sum += val;
            }
        }
        return sum;
    }

    void print_board() const noexcept
    {
        std::cout << std::format("{:05b}\n{:05b}\n{:05b}\n{:05b}\n{:05b}\n",
                                 board & mask,
                                 board >> 5 & (mask),
                                 board >> 10 & (mask),
                                 board >> 15 & (mask),
                                 board >> 20 & (mask));
    }

  private:
    bool is_marked(uint8_t row, uint8_t col) const noexcept
    {
        return board & (uint64_t { 1 } << (row * 5 + col));
    }
    bool set_row_element(uint8_t row, uint8_t col) noexcept
    {
        board |= ((uint64_t { 1 } << row * 5) << col);
        return ((board & (mask << row * 5)) >> row * 5) == mask;
    }
    bool set_col_element(uint8_t row, uint8_t col) noexcept
    {
        board |= ((uint64_t { 1 } << col * 5) << row) << 25;
        return (((board >> 25) & (mask << col * 5)) >> col * 5) == mask;
    }
    uint64_t board = 0; // Used to mark each row and column seperatly, stores all rows consequently from lsb
                        // and then columns
    std::unordered_map<int, std::pair<uint8_t, uint8_t>>
        board_values; // Actual values are stored here with there position
    bool won = false;
};

// TODO can be modified to use std::generator
std::vector<BingoBoard> parse_bingo_boards(std::ifstream& input)
{
    std::vector<BingoBoard> boards;
    std::vector<int> row_storage;
    row_storage.reserve(5);
    std::string line;
    while (input)
    {
        boards.emplace_back();
        for (auto row: std::views::iota(0, 5))
        {
            std::getline(input, line);
            for (auto val: line | std::views::split(" "sv))
            {
                if (*val.begin() != ' ')
                {
                    int temp = 0;
                    std::from_chars(val.data(), val.data() + val.size(), temp);
                    row_storage.push_back(temp);
                }
            }
            boards.back().add_row(row_storage, row);
            row_storage.clear();
        }
        getline(input, line);
    }
    return boards;
}

int main()
{
    std::ifstream input("input/day4");
    std::string line;
    std::getline(input, line);
    std::vector<int> calls;

    for (auto i: line | v::split(","sv))
    {
        calls.emplace_back();
        std::from_chars(i.data(), i.data() + i.size(), calls.back());
    }
    std::getline(input, line);

    auto boards = parse_bingo_boards(input);
    unsigned int total_wins = 0;
    for (auto call: calls)
    {
        for (auto& board: boards)
        {
            if (board.mark(call))
            {
                ++total_wins;
                if(total_wins == 1)
                {
                    std::cout << board.sum_unmarked() * call << "\n";
                }
                if (total_wins == boards.size())
                {
                    std::cout << board.sum_unmarked() * call << "\n";
                    return 0;
                }
            }
        }
    }
}
