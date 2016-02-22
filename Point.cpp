//
// Created by gdhsnlvr on 22.02.16.
//

#include <cmath>
#include "Point.hpp"
#include "DoubleComparations.hpp"

Point::Point(double x, double y, double z)
    : x(x), y(y), z(z)
{

}

Point Point::operator-(const Point &p) const
{
    return Point(x - p.x, y - p.y, z - p.z);
}

Point Point::operator+(const Point &p) const
{
    return Point(x + p.x, y + p.y, z + p.z);
}

Point Point::operator*(double alpha) const
{
    return Point(x * alpha, y * alpha, z * alpha);
}

Point Point::operator/(double alpha) const
{
    return Point(x / alpha, y / alpha, z / alpha);
}

bool Point::operator<(const Point &p) const
{
    return x < p.x || (EQUALS(x, p.x) && y < p.y) || (EQUALS(x, p.x) && EQUALS(y, p.y) && z < p.z);
}

bool Point::operator>(const Point &p) const
{
    return x > p.x || (EQUALS(x, p.x) && y > p.y) || (EQUALS(x, p.x) && EQUALS(y, p.y) && z > p.z);
}

bool Point::operator==(const Point &p) const
{
    return EQUALS(distance(p), 0);
}

bool Point::operator!=(const Point &p) const
{
    return NOTEQUALS(distance(p), 0);
}

double Point::scalar(const Point &p) const
{
    return x * p.x + y * p.y + z * p.z;
}

double Point::distance(const Point &p) const
{
    return (*this - p).length();
}

double Point::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

Point Point::cross(const Point &p) const
{
    return Point(
            y * p.z - z * p.y,
            x * p.z - z * p.x,
            x * p.y - y * p.x
    );
}

Point Point::inv() const
{
    return Point(y, -x, z);
}

Point Point::some_inv() const
{
    Point v;
    if (NOTEQUALS(x, 0)) {
        v.y = 1; v.z = 1;
        v.x = - (y * v.y + z * v.z) / x;
    } else if (NOTEQUALS(y, 0)) {
        v.x = 1; v.z = 1;
        v.y = - (x * v.x + z * v.z) / y;
    } else if (NOTEQUALS(z, 0)) {
        v.x = 1; v.y = 1;
        v.z = - (x * v.x + y * v.y) / z;
    }
    return v;
}

Point Point::normalize() const
{
    return *this / length();
}

Point operator*(double alpha, const Point &p)
{
    return Point(p.x * alpha, p.y * alpha, p.z * alpha);
}

Point operator/(double alpha, const Point &p)
{
    return Point(p.x / alpha, p.y / alpha, p.z / alpha);
}

std::ostream &operator<<(std::ostream &os, const Point &p)
{
    os << "{" << p.x << ", " << p.y << ", " << p.z << "}";
    return os;
}
