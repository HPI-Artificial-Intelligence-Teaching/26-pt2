#include "array_iterator.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>

IteratorExample::IteratorExample() {
    for (int i=0;i<=MAX_POS;i++) {
        ArrayEntry* ent = new ArrayEntry();
        ent->val = i+1;
        arr[i] = ent;
    }
}

IteratorExample::~IteratorExample() {
    for (int i=0;i<=MAX_POS;i++) {
        delete arr[i];
    }
}

int main() {
    IteratorExample* example = new IteratorExample();

    // std::cout << "Array values: ";
    // for (auto it = example->begin(); it != example->end(); ++it) {
    //     std::cout << it->val << " ";
    // }
    // std::cout << std::endl;

    std::cout << "Array values: ";
    for (auto it : *example) {
        std::cout << it.val << " ";
    }
    std::cout << std::endl;

    delete example;
    return 0;
}