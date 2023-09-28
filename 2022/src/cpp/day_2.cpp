#include <fmt/core.h>

#include <cstdlib>
#include <fstream>
#include <ranges>
#include <utility>

#include "frwd_decl.hpp"

enum class Result
{
    LOSS = 0,
    DRAW = 3,
    WIN = 6
};
enum class Move
{
    ROCK = 1,
    PAPER,
    SCI
};

Move move_cast(char move) noexcept
{
    using enum Move;
    switch (move)
    {
        case 'A':
        case 'X': return ROCK;
        case 'B':
        case 'Y': return PAPER;
        case 'C':
        case 'Z': return SCI;
        default: __builtin_unreachable();
    };
}

Result result_cast(char result) noexcept
{
    using enum Result;
    switch (result)
    {
        case 'X': return LOSS;
        case 'Y': return DRAW;
        case 'Z': return WIN;
        default: __builtin_unreachable();
    }
}

int move_to_score(Move move) noexcept
{
    return std::to_underlying(move);
}

int result_to_score(Result result) noexcept
{
    return std::to_underlying(result);
}

int score(Move move, Result result) noexcept
{
    return move_to_score(move) + result_to_score(result);
}

[[nodiscard]] Result player_1_result(Move a, Move b) noexcept
{
    using enum Result;
    using enum Move;
    if (a == b)
        return DRAW;
    switch (a)
    {
        case ROCK:
            if (b == PAPER)
                return LOSS;
            break;
        case PAPER:
            if (b == SCI)
                return LOSS;
            break;
        case SCI:
            if (b == ROCK)
                return LOSS;
            break;
        default: __builtin_unreachable();
    }
    return WIN;
}

[[nodiscard]] Move player_1_move(Move a, Result b) noexcept
{
    using enum Result;
    using enum Move;
    if (b == DRAW)
        return a;
    switch (a)
    {
        case ROCK:
            if (b == LOSS)
                return SCI;
            return PAPER;
            break;
        case PAPER:
            if (b == LOSS)
                return ROCK;
            return SCI;
            break;
        case SCI:
            if (b == LOSS)
                return PAPER;
            return ROCK;
            break;
        default: __builtin_unreachable();
    }
    __builtin_unreachable();
}

[[nodiscard]] int part_1(Move my_move, Move opponent_move) noexcept
{
    return score(my_move, player_1_result(my_move, opponent_move));
}

[[nodiscard]] int part_2(Move opponent_move, Result desired) noexcept
{
    return score(player_1_move(opponent_move, desired), desired);
}

void day2() noexcept
{
    std::string home(std::getenv("HOME"));
    std::ifstream file(home + "/Dropbox/aosc/2022/src/data/day2_input.txt");
    if (!file.is_open())
    {
        fmt::print("Couldn't open input file\n");
        abort();
    }
    std::string line;
    long first_part_sum = 0;
    long second_part_sum = 0;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        char my_turn = line.back();
        char opponent_move = line.front();
        first_part_sum += part_1(move_cast(my_turn), move_cast(opponent_move));
        second_part_sum += part_2(move_cast(opponent_move),
                                  result_cast(my_turn)); // Treat X,Y,Z as results now
    }
    fmt::print("Total score {}\n", first_part_sum);
    fmt::print("Total score {}\n", second_part_sum);
}
