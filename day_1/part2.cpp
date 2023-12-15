#include <fstream>
#include <iostream>
#include <optional>
#include <cmath>

std::optional<int> name_to_digit(std::string_view str)
{
    if (str == "zero")
        return 0;
    if (str == "one")
        return 1;
    if (str == "two")
        return 2;
    if (str == "three")
        return 3;
    if (str == "four")
        return 4;
    if (str == "five")
        return 5;
    if (str == "six")
        return 6;
    if (str == "seven")
        return 7;
    if (str == "eight")
        return 8;
    if (str == "nine")
        return 9;
    return {};
}

int chartoi(char c)
{
    return c - '0';
}
bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}

std::pair<int, std::optional<int>> read_num_from(std::string_view str)
{
    auto begin = str.begin();
    std::optional<int> num;
    auto i = 2;
    for (; i < str.size() && i < 7; ++i)
    {
        auto end = str.begin() + i + 1;
        auto sv = std::string_view(begin, end);
        std::cout << "substr: " << sv << '\n';
        num = name_to_digit(sv);
        if (num)
            break;
    }
    return {i, num};
}

int main()
{
    auto file = std::ifstream("input.txt");

    std::optional<int> num0 = std::nullopt;
    int num1 = 0;
    int num_sum = 0;

    for (std::string line; std::getline(file, line);)
    {
        for (auto cursor{0u}; cursor < line.size();)
        {
            auto [charcount, num] = read_num_from(std::string_view(line.cbegin() + cursor, line.cend()));
            if (num)
            {
                num1 = *num;
                //std::cout << "Str: " << num1 << '\n';

                if (!num0)
                {
                    num0 = num;
                }
                cursor += charcount;
            }
            else
            {
                auto c = line[cursor];
                if (is_digit(c))
                {
                    num1 = chartoi(c);
                    if (!num0)
                        num0 = num1;
                }
                cursor += 1;
            }
        }

        num_sum += (*num0) * 10 + num1;
        num0 = std::nullopt;
    }
    std::cout << num_sum << '\n';
}