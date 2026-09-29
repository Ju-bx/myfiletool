#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char *argv[])
{
    // TODO 1: if the user did not give a filename,
    //         print "Usage: myfiletool <file>" with std::cerr, then return 1
    if (argc < 2)
    {
        std::cerr << "Usage: myfiletool <file>\n";
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename);

    // TODO 2: if the file cannot be opened (hint: if (!file)),
    //         print "Error: cannot open " + filename with std::cerr, then return 1
    if (!file)
    {
        std::cerr << "Error: cannot open " << filename << '\n';
        return 1;
    }

    int lines = 0;
    int words = 0;
    int chars = 0;

    std::string line;
    while (std::getline(file, line))
    {
        // TODO 3: add 1 to lines, add the length of this line to chars
        lines += 1;
        chars += line.size() + 1;
        // TODO 4: use istringstream to count the words in this line
        for (std::size_t i = 0; i < line.size(); i++)
        {
            if (line[i] != ' ' && (i == 0 || line[i - 1] == ' '))
            {
                words += 1;
            }
        }
    }

    std::cout << "File: " << filename << '\n';
    std::cout << "Lines: " << lines << '\n';
    std::cout << "Words: " << words << '\n';
    std::cout << "Characters: " << chars << '\n';

    return 0;
}