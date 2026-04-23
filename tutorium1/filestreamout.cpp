#include <iostream>
#include <fstream> 
#include <stdint.h>
#include <string>   

int main() {
    const std::string filename = "numbers.db";
    std::ofstream outFile;


    outFile.open(filename);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for writing." << std::endl;
        return 1; 
    }

    char c1 = 'L';
    outFile.write(&c1, sizeof(c1));

    char c2 = 'H';
    outFile.write(&c2, sizeof(c2)); 

    uint16_t val1 = 8;
    outFile.write(reinterpret_cast<const char*>(&val1), sizeof(val1));

    char c3 = 'S';
    outFile.write(&c3, sizeof(c3));

    char c4 = 'B';
    outFile.write(&c4, sizeof(c4));

    uint16_t val2 = 5;
    outFile.write(reinterpret_cast<const char*>(&val2), sizeof(val2));

    char c5 = 'R';
    outFile.write(&c5, sizeof(c5));

    char c6 = 'S';
    outFile.write(&c6, sizeof(c6));

    uint16_t val3 = 7;
    outFile.write(reinterpret_cast<const char*>(&val3), sizeof(val3));
    outFile.close();

    if (!outFile) {
         std::cerr << "Error while closing the file '" << filename << "'" << std::endl;
    } else {
        std::cout << "Successfully wrote pattern to '" << filename << "' and closed the file." << std::endl;
    }
    
    return 0; 
}