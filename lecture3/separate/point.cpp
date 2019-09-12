#include <cmath>

#include "point.hpp"

point::point(int x, int y) : x {x}, y {y}
{}

void point::move(int dx, int dy)
{
    x += dx;
	y += dy;
}

double point::getDistance()
{
    return sqrt(x * x + y * y);
}
