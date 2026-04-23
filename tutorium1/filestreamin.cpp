#include <iostream>
#include <fstream> 
#include <stdint.h>
#include <string>

int main() {
    const std::string filename = "numbers.db";
    std::ifstream inFile;

    inFile.open(filename);

    if (!inFile.is_open() || inFile.eof()) {
        std::cerr << "Error: Could not open file '" << filename << std::endl;
        return 1; 
    }

    while (true) {
        char c1;
        char c2;
        uint16_t val1;
        inFile.read(&c1, sizeof(c1));

        if (inFile.eof()) {
            std::cout << "End of file reached." << std::endl;
            break;
        }

        inFile.read(&c2, sizeof(c2));
        inFile.read(reinterpret_cast<char*>(&val1), sizeof(val1));

        std::cout << c1 << c2 << val1 << std::endl;
    }

    inFile.close();

    return 0;
}