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

	friend ostream & operator<<(ostream &, const Point &);
	friend istream & operator>>(istream &, Point &);
};

ostream & operator<<(ostream & os, const Point & p)
{
	os << p.x << ", " << p.y;

	return os;
}

istream & operator>>(istream & is, Point & p)
{
	string s;
	// note that this breaks if we input, e.g. 10,10. Consider is.get(char &) instead for the comma.
	is >> p.x >> s >> p.y;

	if (s != ",")
		is.setstate(ios_base::failbit);

	return is;
}

int main()
{
	Point p {1, -1};
	cout << p << "\n";

	if (cin >> p)
		cout << p << "\n";
}
