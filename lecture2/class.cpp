#include <iostream>
#include <cmath>

using namespace std;

class Point
{
private:
	int x;
	int y;

public:
	Point(int x, int y) : x {x}, y {y}
	{ }

	void move(int dx, int dy)
	{
		x += dx;
		y += dy;
	}

	double getDistance()
	{
		return sqrt(x * x + y * y);
	}
};

int main()
{
	Point p {0, 0};
	cout << p.getDistance() << "\n";

	p.move(1, -1);
	cout << p.getDistance() << "\n";
}
