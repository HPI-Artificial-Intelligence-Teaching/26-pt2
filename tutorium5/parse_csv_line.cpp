// Mini-Tutorial: Pulling values out of a comma-separated string
//
// Demonstrates:
//   - std::istringstream: wraps a string so it can be read like a file
//   - std::getline(stream, token, ',') reads up to (and swallows) the next comma
//   - std::stoi / std::stof convert a string token to int / float
//
// Relevance: In Aufgabe 1.2 every line of airports.csv / routes.csv is one
// record, and the fields are separated by commas. This example parses ONE
// hardcoded line with a fixed sequence of getline calls — for the exercise
// you will want to generalize this (many fields, many lines read from an
// ifstream with the very same getline, just without the third argument).
//
// Compile: clang++ -std=c++17 -o parse_csv_line parse_csv_line.cpp
// Run:     ./parse_csv_line

#include <iostream>
#include <sstream>
#include <string>

int main()
{
    // One record from our repair shop: id, name, status, hours until deadline.
    std::string line = "17,espresso machine,waiting,3.5";

    // An istringstream turns the string into a stream, with a read position
    // that advances — exactly like reading from a file.
    std::istringstream stream(line);
    std::string token;

    // With a third argument, getline stops at that delimiter instead of at
    // the end of the line. The delimiter is consumed but NOT stored.
    std::getline(stream, token, ',');          // token = "17"
    int id = std::stoi(token);                 // "17" -> 17

    std::getline(stream, token, ',');          // token = "espresso machine"
    std::string name = token;

    std::getline(stream, token, ',');          // token = "waiting" — not needed,
                                               // we simply don't use it

    std::getline(stream, token, ',');          // token = "3.5" (no comma follows;
                                               // getline stops at the end instead)
    float hours = std::stof(token);            // "3.5" -> 3.5f

    std::cout << "id:    " << id << std::endl;
    std::cout << "name:  " << name << std::endl;
    std::cout << "hours: " << hours << std::endl;

    return 0;
}

// Things to try / predict before running:
//  (a) What is in `token` after one MORE getline call? And what does that
//      getline return, i.e. what would `while (std::getline(...))` do with it?
//  (b) What happens if you call std::stoi("waiting")? (Look up what stoi
//      throws — and remember tutorium 2: catch it where user data enters.)
//  (c) The fields of a record in airports.csv differ per line, so hardcoding
//      one getline per field won't do. How do you collect ALL tokens of a
//      line, however many there are? (Keep (a) in mind.)
