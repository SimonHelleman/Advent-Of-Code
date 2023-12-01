#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>

std::vector<size_t> find_all_indices_of_substring(const std::string &str, const std::string &sub)
{
    std::vector<size_t> ret;
    size_t pos = str.find(sub, 0);
    while (pos != std::string::npos)
    {
        ret.push_back(pos);
        pos = str.find(sub, pos + 1);
    }

    return ret;
}

int main()
{
    const char *string_digits[] = {
        "one", "two", "three", "four", "five",
        "six", "seven", "eight", "nine",
        "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };

    std::ifstream file("input.txt");

    int sum = 0;

    while (!file.eof())
    {
        std::string line;
        getline(file, line);

        // (digit, idx)
        std::vector<std::pair<size_t, size_t>> digit_position;

        std::cout << line << '\n';
        for (int i = 0; i < 18; ++i)
        {
            const auto pos = find_all_indices_of_substring(line, string_digits[i]);

            for (const auto &index : pos)
            {
                digit_position.emplace_back((i % 9) + 1, index);
            }
        }

        int first_index = digit_position[0].second;
        int first_digit = digit_position[0].first;

        int last_index = digit_position[0].second;
        int last_digit = digit_position[0].first;

        for (const auto &i : digit_position)
        {
            if (i.second < first_index)
            {
                first_digit = i.first;
                first_index = i.second;
            }

            if (i.second > last_index)
            {
                last_digit = i.first;
                last_index = i.second;
            }
            //std::cout << "(" << i.first << " " << i.second << ") ";
        }

        int num = first_digit * 10 + last_digit;
        //std::cout << num << '\n';
        sum += num;
    } 

    std::cout << sum << '\n';
    return 0;
}