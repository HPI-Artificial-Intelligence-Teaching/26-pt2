// Tutorial: Reading and tokenizing user input from the command line
//
// Demonstrates:
//   - Reading a full line with std::getline (vs. std::cin >>)
//   - Iterating over a std::string character by character
//   - Accumulating digits/dots into a number token ("consume while it matches")
//   - Converting a string token to a float with std::stof
//   - Catching bad input with try/catch
//
// Compile: g++ -std=c++17 -o token_parsing token_parsing.cpp
// Run:     ./token_parsing

#include <iostream>
#include <stdexcept>
#include <string>

// Returns true if c can be part of a floating-point number literal.
bool is_number_char(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}

int main()
{
    std::string line;

    std::cout << "Enter a sequence of numbers separated by spaces, e.g.: 3.14  2  0.5" << std::endl;
    std::cout << ">>> ";

    // std::getline reads everything up to (and discarding) the newline.
    // Unlike `std::cin >> x`, it does NOT skip leading whitespace and
    // does NOT stop at spaces, so we get the whole line as one string.
    std::getline(std::cin, line);

    std::cout << "You entered: \"" << line << "\"" << std::endl;
    std::cout << "Tokens found:" << std::endl;

    size_t i = 0;
    while (i < line.size())
    {
        char c = line[i];

        if (c == ' ')
        {
            // Skip whitespace between tokens.
            ++i;
        }
        else if (is_number_char(c))
        {
            // Start of a number token: consume every character that can
            // belong to a floating-point literal (digits and '.').
            std::string token;
            while (i < line.size() && is_number_char(line[i]))
            {
                token += line[i];
                ++i;
            }

            // Convert the accumulated string to a float.
            // std::stof throws std::invalid_argument if the string is not a
            // valid number (e.g. "3..14"), and std::out_of_range if it
            // overflows — always catch these at the boundary where user
            // input enters the program.
            try
            {
                float value = std::stof(token);
                std::cout << "  number token: \"" << token << "\" -> " << value << std::endl;
            }
            catch (const std::invalid_argument&)
            {
                std::cout << "  invalid number token: \"" << token << "\"" << std::endl;
            }
        }
        else
        {
            std::cout << "  unexpected character: '" << c << "'" << std::endl;
            ++i;
        }
    }

    return 0;
}
