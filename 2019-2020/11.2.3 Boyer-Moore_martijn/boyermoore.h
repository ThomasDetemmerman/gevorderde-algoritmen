#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream> // DEBUG
#include <queue>
#include <sstream>
#include <string>

// Good Suffix heuristic
//
//  O(p^2)
//
//      i                   |   0   1   2   3   4   5   6
//      p-i-1               |   6   5   4   3   2   1   0
//      --------------------+-------------------------------
//      p[i]                |   a   b   b   a   b   a   b
//      --------------------+-------------------------------
//      suffix[i]           |   2   1   3   2   1   0   0
//      k                   |   /   /   /   2   3   4   6
//      i+1-k               |   /   /   /   2   2   2   1
//      verschuiving[i]     |   5   5   5   2   2   2   1
//                             ~~~~~~~~~~~             ~~~
//                                  |                   |
//                                  |                   V
//                                  |
//                                  |         Er is geen juiste suffix
//                                  V
//
//              Het juiste suffix komt niet meer in P voor:
//              p - s[0] = 7 - 2 = 5
//

using uchar = unsigned char;
constexpr size_t alphabet_size = (1 << (sizeof(uchar) * 8));

class BoyerMoore
{
public:
    BoyerMoore() = delete;
    BoyerMoore(const std::string& pattern);
    BoyerMoore(const BoyerMoore& other) = default;
    BoyerMoore(BoyerMoore&& other) = default;
    BoyerMoore& operator=(const BoyerMoore& other) = default;
    BoyerMoore& operator=(BoyerMoore&& other) = default;
    virtual ~BoyerMoore() = default;

    std::queue<int> search(const std::string& text) const;
    std::vector<int> get_bad_character_table() const;
    std::vector<int> get_weak_common_suffix_table() const;
    std::vector<int> get_weak_good_suffix_shift_table() const;
    std::string to_string() const;

private:
    int reverse_pattern_index(int index) const;

    std::string pattern;
    std::vector<int> bad_character_table;
    std::vector<int> weak_common_suffix_table;
    std::vector<int> strong_common_suffix_table;
    std::vector<int> weak_good_suffix_shift_table;
    std::vector<int> strong_good_suffix_shift_table;
};

BoyerMoore::BoyerMoore(const std::string& pattern): pattern{pattern}, bad_character_table(alphabet_size, -1), weak_common_suffix_table(pattern.length(), 0){
    for (int i = 0; i < pattern.size(); i++){
        bad_character_table[static_cast<uchar>(pattern[i])] = i;
    }

    int common_suffix_length = 0;
    for (int i = (weak_common_suffix_table.size() - 2); i >= 0; i--)
    {

        while ((common_suffix_length > 0) && (pattern[i] != pattern[reverse_pattern_index(common_suffix_length)])){
            common_suffix_length = weak_common_suffix_table[reverse_pattern_index(common_suffix_length) + 1];
        }

        if (pattern[i] == pattern[reverse_pattern_index(common_suffix_length)]){
            common_suffix_length++;
        }
        weak_common_suffix_table[i] = common_suffix_length;
    }

    weak_good_suffix_shift_table = std::vector<int>(pattern.length(), (pattern.length() - weak_common_suffix_table[0]));
    for (int i = 0; i < weak_good_suffix_shift_table.size(); i++){
        int suffix_index = (pattern.length() - weak_common_suffix_table[i] - 1);
        weak_good_suffix_shift_table[suffix_index] = (suffix_index + 1 - i);
    }
}

int BoyerMoore::reverse_pattern_index(int index) const
{
    return (pattern.length() - index - 1);
}

std::vector<int> BoyerMoore::get_bad_character_table() const
{
    return bad_character_table;
}

std::vector<int> BoyerMoore::get_weak_common_suffix_table() const
{
    return weak_common_suffix_table;
}

std::vector<int> BoyerMoore::get_weak_good_suffix_shift_table() const
{
    return weak_good_suffix_shift_table;
}

std::queue<int> BoyerMoore::search(const std::string& text) const
{
    if (text.size() < pattern.size())
    {
        return std::queue<int>{};
    }

    std::queue<int> results;

    int text_i = 0;

    while (text_i <= (text.size() - pattern.size()))
    {
        int pattern_i = (pattern.size() - 1);
        while ((pattern_i >= 0) && (pattern[pattern_i] == text[text_i + pattern_i]))
        {
            pattern_i--;
        }

        int shift = 0;

        if (pattern_i < 0)
        {
            results.push(text_i);

            if ((text_i + pattern.size()) < text.size())
            {
                uchar desired_character = text[text_i + pattern.size()];
                shift = (pattern.size() - bad_character_table[desired_character]);
            }
        }
        else
        {
            uchar desired_character = text[text_i + pattern_i];
            shift = (pattern_i - bad_character_table[desired_character]);
        }

        text_i += std::max(shift, 1);
    }

    return results;
}

std::string BoyerMoore::to_string() const
{
    std::stringstream out;
    constexpr int field_width = 4;
    constexpr int header_width = 40;

    out << std::setw(header_width) << "i";
    for (int i = 0; i < pattern.size(); i++)
    {
        out << std::setw(field_width) << i;
    }
    out << std::endl;

    out << std::setw(header_width) << "reverse i";
    for (int i = 0; i < pattern.size(); i++)
    {
        out << std::setw(field_width) << reverse_pattern_index(i);
    }
    out << std::endl;

    out << std::setw(header_width) << "pattern";
    for (char c : pattern)
    {
        out << std::setw(field_width) << c;
    }
    out << std::endl;

    out << std::setw(header_width) << "weak common suffix";
    for (int i : weak_common_suffix_table)
    {
        out << std::setw(field_width) << i;
    }
    out << std::endl;

    out << std::setw(header_width) << "weak shift";
    for (int i : weak_good_suffix_shift_table)
    {
        out << std::setw(field_width) << i;
    }
    out << std::endl;

    return out.str();
}

#endif
