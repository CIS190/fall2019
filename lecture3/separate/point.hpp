#ifndef POINT_HPP
#define POINT_HPP

class point
{
private:
    int x;
    int y;

public:
    point(int, int);

    void move(int, int);
    double getDistance();
};

#endif
