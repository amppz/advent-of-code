#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <ranges>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string_view>

int main()
{
    constexpr size_t MIN_COLON_POS = 5;

    auto file = std::ifstream("input.txt");
    size_t total_score = 0;

    for (std::string line; std::getline(file, line);)
    {
        std::set<long> winning_numbers;
        auto colon_idx = line.find(':', MIN_COLON_POS);
        auto pipe_idx = line.find('|', colon_idx);

        if (colon_idx == line.npos)
        {
            std::cerr << "Failed to find colon\n";
            return -1;
        }

        auto offset = 1;
        for (auto i = colon_idx + 1; i < pipe_idx - 1; ++i)
        {
            char *end;
            auto begin = line.data() + i;

            auto num = std::strtol(begin, &end, 10);
            if (begin == end)
            {
                std::cerr << "Failed to convert winning number\n";
                return -2;
            }
            winning_numbers.insert(num);
            i += end - begin;
        }

        std::set<long> held_numbers;

        for (auto i = pipe_idx + 1; i < line.size();)
        {
            char *end;
            auto begin = line.data() + i;

            auto num = std::strtol(begin, &end, 10);
            if (begin == end)
            {
                std::cerr << "Failed to convert held number\n";
                return -3;
            }
            held_numbers.insert(num);

            i += end - begin;
        }

        std::vector<int> out{};
        std::ranges::set_intersection(winning_numbers, held_numbers, std::back_inserter(out));
        auto length = out.size();
        if (length)
        {
            total_score += 1 << (length - 1);
        }
    }
    std::cout << "Total: " << total_score << '\n';
    return total_score;
}
