//
// Created by gdhsnlvr on 22.02.16.
//

#ifndef STONES_POINT_HPP
#define STONES_POINT_HPP

#include <iostream>

class Point
{
public:
    double x, y, z;

    Point(double x = 0, double y = 0, double z = 0);

    Point operator - (const Point &p) const;
    Point operator + (const Point &p) const;
    Point operator * (double alpha) const;
    Point operator / (double alpha) const;

    bool operator < (const Point &p) const;
    bool operator > (const Point &p) const;
    bool operator == (const Point &p) const;
    bool operator != (const Point &p) const;

    double scalar(const Point &p) const;
    double distance(const Point &p) const;
    double length() const;

    Point cross(const Point &p) const;
    Point inv() const; // 2d normal
    Point some_inv() const; // some of 3d normal vector (this->some_inv().scalar(*this) == 0)
    Point normalize() const;
};

Point operator * (double alpha, const Point &p);
Point operator / (double alpha, const Point &p);
std::ostream &operator << (std::ostream &os, const Point &p);

#endif //STONES_POINT_HPP