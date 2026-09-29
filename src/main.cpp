#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>

std::string normalize(const std::string &word)
{
    std::string result;
    for (char ch : word)
    {
        if (std::isalnum(ch))
        {
            result += std::tolower(ch);
        }
    }
    return result;
}

void add_word(std::map<std::string, int >& freq, const std::string& raw)
{
    std::string cur = normalize(raw);
    if (!cur.empty())
    {
        freq[cur] += 1;
    }   
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Usage: myfiletool <file>\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "Error: cannot open " << filename << '\n';
        return 1;
    }

    int lines = 0;
    int words = 0;
    int chars = 0;
    std::map<std::string, int> freq;
    std::string line;
    while (std::getline(file, line))
    {
        lines += 1;
        chars += line.size() + 1;

        std::string cur;
        for (std::size_t i = 0; i < line.size(); i++)
        {
            if (line[i] != ' ' && (i == 0 || line[i - 1] == ' '))
            {
                // start of a word
                words += 1;
                cur.clear();
                cur += line[i];
            }
            else if (line[i] != ' ')
            {
                // middle of a word
                cur += line[i];
            }
            else if (!cur.empty())
            {
                // space after a word: the word is complete
                add_word(freq, cur);
                cur.clear();
            }
        }

        // last word of the line (no space after it)
        if (!cur.empty())
        {
           add_word(freq, cur);
        }
    }
    const std::size_t N = 5;
    std::vector<std::pair<std::string, int>> top;
    for (const auto &[w, c] : freq)
    {
        if (top.size() < N)
        {
            top.push_back({w, c});
        }
        else if (c > top.back().second)
        {
            top.back() = {w, c};
        }
        else
        {
            continue;
        }
        // move the last to right place
        std::size_t i = top.size() - 1;
        while (i > 0 && top[i].second > top[i - 1].second)
        {
            std::swap(top[i], top[i - 1]);
            i--;
        }
    }
    std::cout << "File: " << filename << '\n';
    std::cout << "Lines: " << lines << '\n';
    std::cout << "Words: " << words << '\n';
    std::cout << "Characters: " << chars << '\n';

    std::cout << "\nWord frequency:\n";
    for (const auto &[w, c] : freq)
    {
        std::cout << w << ": " << c << '\n';
    }

    std::cout << "\nTop " << N << " words:\n";
    for (const auto &[w, c] : top)
    {
        std::cout << w << ": " << c << '\n';
    }

    return 0;
}