#include <iostream>

using namespace std;

class Screen {
public:
    typedef std::string::size_type pos;
    Screen() = default;
    Screen(pos ht, pos wd, char c):height(ht), width(wd), contents(ht * wd, c){}
    Screen(pos ht, pos wd):height(ht), width(wd), contents(ht, wd) {}
    char get() const{
        return contents[cursor];
    }
    inline char get(pos ht, pos wd) const;

    Screen &move(pos r, pos col) {
        cursor = r * height + col;
        return *this;
    }
    Screen &set(char);
    Screen &set(pos, pos, char);
    const Screen &display(ostream &os) const {do_display(os); return *this;}//*this 是常对象，所以返回值为常引用
    Screen &display(ostream &os)  {do_display(os); return *this;}
private:
    void do_display (ostream& os) const{
        os << contents;
    }
    pos cursor = 0;
    pos height = 0, width = 0;
    string contents;
};

inline Screen &Screen::set(char c) {
    contents[cursor] = c;
    return *this;
}

inline Screen &Screen::set(pos r, pos col, char c) {
    contents[r * height + col] = c;
    return *this;
}

int main () {
    Screen s(5, 5, 'X');
    s.move(4, 0).set('#').display(cout);
    cout << "\n";
    s.display(cout);
    cout << '\n';
    return 0;
}