//
// Created by gdhsnlvr on 20.02.16.
//

#ifndef STONES_TRIANGULATION_HPP
#define STONES_TRIANGULATION_HPP

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "DoubleComparations.hpp"
#include "Point.hpp"

namespace triangulation
{
    enum class Classification
    {
        Left, Right, Between, InFront, InBack, Begin, End
    };

    Classification classify(const Point &p, const Point &begin, const Point &end);

    struct FrontierEdge
    {
        Point *first, *second;

        bool operator<(const FrontierEdge &e) const;
        void flip();
    };

    struct Segment
    {
        Point begin, end;

        Segment rotate() const;
        bool intersect(const Segment &s, double &t) const;
    };

    struct Triangle
    {
        Point *a, *b, *c;
    };

    class DeluanayTriangulation
    {
    protected:
        size_t n;
        std::vector<Point> *m_points;

    public:
        DeluanayTriangulation(std::vector<Point> *points);
        std::vector<Triangle> compute();

    protected:
        FrontierEdge findHullEdge() const;
        bool findMatePoint(const FrontierEdge &e, Point *&res) const;
    };
}

#endif //STONES_TRIANGULATION_HPP
