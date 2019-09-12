#include <iostream>

using namespace std;

class point
{
public:
    int x = 0;
    int y = 0;

public:
    point() {}
    point(int x) : x {x} {}
    point(int x, int y) : x {x}, y {y} {}
};

int main()
{
    point p1;

    cout << p1.x << " " << p1.y << "\n";
}
