#include <cmath>

class point
{
private:
	int x;
	int y;

public:
	point(int x, int y) : x {x}, y {y}
	{ }

	void move(int dx, int dy)
	{
		x += dx;
		y += dy;
	}

	double getDistance() const
	{
        // x++; // not allowed
		return sqrt(x * x + y * y);
	}
};

int main()
{
    const point p {0, 0};
    // p.move(1, 1); // not allowed
    p.getDistance();
}
