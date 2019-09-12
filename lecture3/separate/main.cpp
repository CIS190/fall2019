#include <iostream>

#include "point.hpp"

using namespace std;

int main()
{
    point p {0, 0};
    cout << p.getDistance() << "\n";

    p.move(1, -1);
    cout << p.getDistance() << "\n";
}
