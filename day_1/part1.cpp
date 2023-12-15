#include <fstream>
#include <iostream>
#include <utility>
#include <optional>
#include <ranges>

int chartoi(char c)
{
    return c - '0';
}
bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

int main()
{
    auto file = std::ifstream("input.txt");

    std::optional<int> num0;
    int num1;

    int num_sum = 0;
    for (std::string line; std::getline(file, line);)
    {
        for (auto c : line | std::views::filter(is_digit))
        {
            num1 = chartoi(c);
            if (!num0)
            {
                num0 = num1;
            }
        }
        num_sum += (*num0) * 10 + num1;
        num0 = std::nullopt;
    }
    std::cout << num_sum << '\n';
}
