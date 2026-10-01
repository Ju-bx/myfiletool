#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>

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

struct FileStats {
    int lines = 0;
    int words = 0;
    int chars = 0;
    std::map<std::string, int> freq;
};

using WordCount = std::pair<std::string, int>;

void merge(FileStats& total, const FileStats& s)
{
    total.lines += s.lines;
    total.words += s.words;
    total.chars += s.chars;

    for (const auto& [w, c] : s.freq) {
        total.freq[w] += c;
    }
}

bool count_file(const std::string& filename, FileStats& stats)
{
    std::ifstream file(filename);
    if (!file) { return false; }

    std::string line;
    while (std::getline(file, line))
    {
        stats.lines += 1;
        stats.chars += line.size() + 1;

        std::string cur;
        for (std::size_t i = 0; i < line.size(); i++)
        {
            if (line[i] != ' ' && ( i ==0 || line[i - 1] == ' '))
            {
                // start of a word
                stats.words += 1;
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
                add_word(stats.freq, cur);
                cur.clear();
            }
        }

        // last word of the line (no space after it)
        if (!cur.empty())
        {
           add_word(stats.freq, cur);
        }
    }
    return true;
}

std::vector<WordCount> top_words(const std::map<std::string, int>& freq, std::size_t n)
{
    std::vector<WordCount> top;
    for (const auto &[w, c] : freq)
    {
        if (top.size() < n)
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
    return top;
}

void print_report(const std::string& title, const FileStats& stats, const std::vector<WordCount>& top)
{
    std::cout << title << '\n';
    std::cout << "Lines: " << stats.lines << '\n';
    std::cout << "Words: " << stats.words << '\n';
    std::cout << "Characters: " << stats.chars << '\n';

    std::cout << "\nTop " << top.size() << " words:\n";
    for (const auto &[w, c] : top)
    {
        std::cout << w << ": " << c << '\n';
    }
    std::cout << '\n';
}

int main(int argc, char *argv[])
{

    std::size_t n = 5;
    std::vector<std::string> files;
    if (argc < 2)
    {
        std::cerr << "Usage: myfiletool <file>\n";
        return 1;
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            std::string arg = argv[i];
            if (arg == "--top")
            {
                if (i+1 >= argc)
                {   
                    std::cerr << "Error: --top need a number\n";
                    return 1;
                }
                int num = 0;
                try{
                    num = std::stoi(argv[i+1]);
                }catch (const std::invalid_argument& e) {
                    std::cerr << "nuused number\n" << std::endl;
                    return 1;
                } catch (const std::out_of_range& e) {
                    std::cerr << "out of int range\n" << std::endl;
                    return 1;
                }
                if (num <= 0){
                    std::cerr << "--top need be a positive number\n";
                    return 1;
                }
                n = static_cast<std::size_t>(num);
                            

            }
            else if (arg.find(".txt") != std::string::npos){
                files.push_back(arg);
            }

        }
        
    }
    FileStats total;
    bool failed = false;
    for (std::string filename: files){
        FileStats stats;
        
        if (!count_file(filename, stats)) {
            std::cerr << "Error: cannot open " << filename << '\n';
            failed = true;
            continue;
        }
        merge(total, stats);
        
        std::vector<WordCount> top = top_words(stats.freq, n);
        
        print_report("File: " + filename, stats, top);
    }
    if (files.size() >1){
        std::vector<WordCount> total_top = top_words(total.freq, n);
        print_report("=========total=========", total, total_top);
    }
    
    return failed ? 1 : 0;
}