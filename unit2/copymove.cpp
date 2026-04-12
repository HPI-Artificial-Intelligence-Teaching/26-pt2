#include <iomanip>
#include <iostream>

using namespace std;

class MyInt {
    int* value;  // heap-allocated integer (to demonstrate copy vs. move semantics)
   public:
    MyInt(int i) : value{new int(i)} {
        cout << "Ordinary constructor MyInt(int = " << i << ")" << endl;
    }
    MyInt() : value{new int(0)} {
        cout << "Default constructor MyInt()" << endl;
    }
    MyInt(const MyInt& mi) : value{new int(*mi.value)} {
        cout << "Copy constructor MyInt(MyInt& = " << *mi.value << ")" << endl;
    }
    MyInt(MyInt&& mi) : value{mi.value} {
        mi.value = nullptr;
        cout << "Move constructor MyInt(MyInt&& = " << *value << ")" << endl;
    }
    MyInt& operator=(const MyInt& mi) {
        cout << "Copy assignment MyInt& operator=(const MyInt& = " << *mi.value << ")" << endl;
        *value = *mi.value;
        return (*this);
    }
    MyInt& operator=(MyInt&& mi) {
        cout << "Move assignment MyInt& operator=(MyInt&& = " << *mi.value << ")" << endl;
        delete value;
        value = mi.value;
        mi.value = nullptr;
        return (*this);
    }
    ~MyInt() {
        if (value)
            cout << "Destructor ~MyInt(= " << *value << ")" << endl;
        else
            cout << "Destructor ~MyInt(= moved)" << endl;
        delete value;
    }

    int val() { return *value; }
};

MyInt f(MyInt& mi) {
    cout << "\t\tMyInt tmp { mi.val() + 25 };" << endl;
    MyInt tmp{mi.val() + 25};

    cout << "\t\treturn (tmp);" << endl;
    return (tmp);
}

// main entry point of the program
int main(void) {
    cout << "\tMyInt m1;" << endl;
    MyInt m1;
    cout << "\tMyInt m2 { 10 };" << endl;
    MyInt m2{10};
    cout << "\tMyInt m3 = 12;" << endl;
    MyInt m3 = 12;
    cout << "\tMyInt m4 { m3 };" << endl;
    MyInt m4{m3};

    cout << "\tm1 = m2;" << endl;
    m1 = m2;

    cout << "\tm3 = f(m1);" << endl;
    m3 = f(m1);

    cout << "\tMyInt m5{move(f(m3))};" << endl;
    MyInt m5{move(f(m3))};

    cout << "\treturn (0);" << endl;
    return (0);
}