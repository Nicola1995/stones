//
// Created by gdhsnlvr on 20.02.16.
//

#ifndef STONES_TRIANGULATION_HPP
#define STONES_TRIANGULATION_HPP

#include <algorithm>
#include <vector>
#include <set>

struct Point
{
    double x, y;

    Point operator - (const Point &p) const
    {
        return {x - p.x, y - p.y};
    }

    Point operator + (const Point &p) const
    {
        return {x + p.x, y + p.y};
    }

    Point operator * (double alpha) const
    {
        return {x * alpha, y * alpha};
    }

    Point inv() const
    {
        return {-y, x};
    }

    double distance(const Point &a) const
    {
        return std::sqrt((a.x - x) * (a.x - x) + (a.y - y) * (a.y - y));
    }

    double cross(const Point &a) const
    {
        return x * a.y - y * a.x;
    }

    bool operator < (const Point &p) const
    {
        return y < p.y || (y == p.y && x < p.x);
    }

    bool operator > (const Point &p) const
    {
        return y > p.y || (y == p.y && x > p.x);
    }
};

namespace triangulation
{
    struct Edge
    {
        Point *first, *second;

        Edge (Point *first, Point *second);
        void flip();
        bool operator<(const Edge &e) const;
    };

    struct Segment
    {
        Point begin, end;

        Segment rotate() const;
        bool intersect(const Segment &s, double t) const;
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
        std::vector<size_t> index;

    public:
        DeluanayTriangulation(std::vector<Point> *points);
        virtual ~DeluanayTriangulation();

        std::vector<Triangle> compute();

    protected:
        enum class Classification
        {
            Left,
            Right,
            Between,
            OutsideLine,
            Error
        };
        Classification classify(const Point &a, const Point &begin, const Point &end) const;

        Edge findHullEdge() const;
        bool findMatePoint(const Edge &e, Point *&res) const;
    };
}

#endif //STONES_TRIANGULATION_HPP
