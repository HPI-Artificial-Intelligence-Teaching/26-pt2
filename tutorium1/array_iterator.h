#ifndef ARRAY_ITERATOR_H
#define ARRAY_ITERATOR_H

#include <stdint.h>
#include <stdio.h>
#include <fstream>

struct ArrayEntry {
    int val;
};

class IteratorExample {
    public: 
        ArrayEntry* arr[10];
        const int MAX_POS = 9;

        struct Iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = ArrayEntry;
            using pointer = ArrayEntry*;
            using reference = ArrayEntry&;

            reference operator*() const { return *(container.arr[arr_pos]); };
            pointer operator->() { return container.arr[arr_pos]; };
            Iterator& operator++() { arr_pos += 1; return *this; };
            Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; };

            friend bool operator== (const Iterator& a, const Iterator& b) { return a.arr_pos == b.arr_pos; };
            friend bool operator!= (const Iterator& a, const Iterator& b) { return a.arr_pos != b.arr_pos; }; 

            Iterator(IteratorExample& container, int arr_pos) : arr_pos(arr_pos), container(container) {}

            private:
                int arr_pos;
                IteratorExample& container;
        };
        Iterator begin() { return Iterator(*this, 0); };
        Iterator end() { return Iterator(*this, MAX_POS + 1); };
        IteratorExample();
        ~IteratorExample();
};

#endif /* ARRAY_ITERATOR_H */