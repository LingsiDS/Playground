#include <string>
#include <iostream>
using namespace std;

class Person {
private:
    string name;
    string address;
public:
    using int_ = int;
    friend istream& read(istream &is, Person &rhs);

    Person() = default;
    Person(const string &name, const string &address) {
        this->name = name;
        this->address = address;
    }

    // Person(istream &is) {
    //     read(is, *this);
    // ｝

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
    os << rhs.getName() << " " << rhs.getAddress();
    return os;
}



int main() {
    Person p("lingsi", "suzhou");
    print(cout, p);
    Person p2;
    read(cin, p2);
    print(cout, p2);
    Person::int_ a = 1;
    cout << a << endl;
    return 0;
}