#include <string>
#include <iostream>
using namespace std;

class Person {
public:
    string name;
    string address;
public:
    string getName() const {
        return name;
    }
    string getAddress() const {
        return address;
    }
};

istream& read(istream &is, Person &rhs) {
    is >> rhs.name >> rhs.address;
    return is;
}
ostream &print(ostream &os, const Person &rhs) {
    os << rhs.getName() << rhs.getAddress();
    return os;
}

int main() {

    return 0;
}