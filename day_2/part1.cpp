#include <iostream>
#include <fstream>
#include <string_view>
#include <optional>
#include <vector>
#include <string>
#include <set>

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
    int red;
    int green;
    int blue;
};

constexpr colours colour_max = {.red = 12, .green = 13, .blue = 14};

int main()
{

    auto file = std::ifstream("input");
    auto valid_sum = 0ull;

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
        bool valid = true;

        while (valid && has_next)
        {
            if (*game_num == 81)
            {
                std::cout << r.remaining() << '\n';
            }
            valid = false;
            has_next = false;
            colours rgb;

            auto num = r.read_ll();
            if (!num)
            {
                std::cerr << "Expcted a number\n";
                return -3;
            }
            if (*game_num == 81)
            {
                std::cout << r.remaining() << '\n';
            }

            r.skip_set({' '});

            auto const str = r.read_word();

            if (str == "blue")
            {
                rgb.blue = *num;
                if (rgb.blue > colour_max.blue)
                {
                    continue;
                }
            }
            else if (str == "green")
            {
                rgb.green = *num;
                if (rgb.green > colour_max.green)
                {
                    continue;
                }
            }
            else if (str == "red")
            {
                rgb.red = *num;
                if (rgb.red > colour_max.red)
                    continue;
            }
            else
            {
                std::cerr << "Unknown colour: " << str << '\n';
                return -4;
            }
        
            valid = true;
            if (r.can_read())
            {
                if (r.expect(", ") || r.expect("; "))
                    has_next = true;
            }
        }
        if (valid)
            valid_sum += *game_num;
        else
            std::clog << *game_num << " is invalid\n";
    }
    std::cout << "Sum: " << valid_sum << '\n';
}