#include <iostream>
#include <fstream>
#include <string_view>
#include <optional>
#include <vector>
#include <string>
#include <set>
#include <limits>

class reader
{
public:
    reader(std::string_view str) : m_str(str), m_cursor(str.begin()) {}

    bool expect(std::string_view str)
    {
        if (std::equal(str.begin(), str.end(), m_cursor))
        {
            m_cursor += str.size();
            return true;
        }
        return false;
    }
    std::optional<long long> read_ll()
    {
        char *end;
        auto res = std::strtoll(m_cursor, &end, 10);
        if (m_cursor == end)
        {
            return std::nullopt;
        }
        else
        {
            m_cursor += end - m_cursor;
            return res;
        }
    }
    std::string_view read_word()
    {
        auto begin = m_cursor;
        auto &it = m_cursor;
        for (; it != m_str.end() && std::isalpha(*it); ++it)
            ;
        return {begin, m_cursor};
    }
    void skip_set(std::set<char> set)
    {
        auto begin = m_cursor;
        auto it = begin;
        for (; it != m_str.end() && set.contains(*it); ++it)
            ;
        m_cursor = it;
    }
    bool can_read() const
    {
        return m_cursor - m_str.begin() < m_str.size();
    }
    std::string_view remaining()
    {
        return std::string_view(m_cursor, m_str.end());
    }

private:
    std::string_view::const_iterator m_cursor;
    std::string_view const m_str;
};

struct colours
{
    long long red;
    long long green;
    long long blue;
};

auto constexpr min_long = std::numeric_limits<long long>::min();

constexpr colours colour_max = {.red = 12, .green = 13, .blue = 14};

int main()
{

    auto file = std::ifstream("input.txt");
    auto power_set_sum = 0ull;

    for (std::string line; std::getline(file, line);)
    {
        reader r(line);
        if (!r.expect("Game "))
        {
            std::cerr << "Expected 'Game '\n";
            return -1;
        }
        auto game_num = r.read_ll();
        if (!r.expect(": "))
        {
            std::cerr << "Expected ': '\n";
            return -2;
        }
        bool has_next = true;
        colours rgb = {min_long, min_long, min_long};

        while (has_next)
        {
            has_next = false;

            auto num = r.read_ll();
            if (!num)
            {
                std::cerr << "Expcted a number\n";
                return -3;
            }
            r.skip_set({' '});

            auto const str = r.read_word();
            std::cout << "Num: " << *num << '\n';
            if (str == "blue")
                rgb.blue = std::max(*num, rgb.blue);
            else if (str == "green")
                rgb.green = std::max(*num, rgb.green);
            else if (str == "red")
                rgb.red = std::max(*num, rgb.red);
            else
            {
                std::cerr << "Unknown colour: " << str << '\n';
                return -4;
            }

            if (r.can_read())
            {
                if (r.expect(", ") || r.expect("; "))
                    has_next = true;
            }
        }
        if (rgb.red == min_long)
            rgb.red = 0;
        if (rgb.green == min_long)
            rgb.green = 0;
        if (rgb.blue == min_long)
            rgb.blue = 0;

        power_set_sum += rgb.red * rgb.green * rgb.blue;
    }
    std::cout << "Sum: " << power_set_sum << '\n';
}