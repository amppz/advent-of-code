#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <format>

struct node
{

    explicit node(int num = -1) : m_num(num), m_id(counter++)
    {
        m_id = counter++;
    }
    node(const node &n)
    {
        m_id = n.m_id;
        m_num = n.m_num;
    }
    node &operator=(const node &n)
    {
        m_id = n.m_id;
        m_num = n.m_num;
        return *this;
    }
    node(node &&n)
    {
        m_id = n.m_id;
        m_num = n.m_num;
    }
    bool operator==(node const &n) const
    {
        return m_id == n.m_id;
    }

private:
    static int counter;

public:
    int m_num;
    int m_id;
};

int node::counter = 0;

struct coords
{
    size_t i;
    size_t j;
};

int main()
{
    auto file = std::ifstream("input.txt");
    std::vector<std::vector<node>> num_matrix;
    std::vector<coords> symbol_coords;
    auto line_no = 0u;
    size_t length = 0;

    for (std::string line; std::getline(file, line); line_no += 1)
    {
        auto idx = 0;
        length = line.size();
        num_matrix.emplace_back(length);

        while (idx < length)
        {
            for (; line[idx] == '.' && idx < length; ++idx);

            if (idx >= line.size())
                continue;

            if (!std::isdigit(line[idx]))
            {
                symbol_coords.emplace_back(line_no, idx);
                idx += 1;
            }
            else
            {
                char *end;
                auto start_idx = idx;
                auto begin = line.begin().base() + idx;

                auto const parsed_num = std::strtoull(begin, &end, 10);

                auto end_idx = start_idx + end - begin;

                if (end != begin)
                {
                    auto new_node = node(parsed_num);
                    for (auto i = start_idx; i < end_idx; ++i)
                    {
                        num_matrix[line_no][i] = new_node;
                    }
                }
                idx = end_idx;
            }
        }
    }
    
    auto gear_sum = 0ll;

    for (auto [i, j] : symbol_coords)
    {
        auto row_min = std::max<long long>(0, i - 1);
        auto row_max = std::min<long long>(i + 2, line_no);

        auto gear_ratio = 1ll;
        std::map<int, int> adjacent_nums;

        for (auto row = row_min; row < row_max; ++row)
        {
            auto col_min = std::max<long long>(0, j - 1);
            auto col_max = std::min<long long>(j + 2, length);

            for (auto col = col_min; col < col_max; ++col)
            {
                auto &member = num_matrix[row][col];
                if (member.m_num != -1)
                {
                    adjacent_nums[member.m_id] = member.m_num;
                }
            }
        }

        if (adjacent_nums.size() == 2){
            for (auto [key, val] : adjacent_nums){
                gear_ratio *= val;
            }
            gear_sum += gear_ratio;
        }

    }
    std::cout << gear_sum << std::endl;

}