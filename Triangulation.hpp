//
// Created by gdhsnlvr on 20.02.16.
//

#ifndef STONES_TRIANGULATION_HPP
#define STONES_TRIANGULATION_HPP

#include <algorithm>
#include <vector>
#include <set>

#define EPSILON (1e-9)
#define EQUALS(a, b) (abs(a - b) < EPSILON)
#define BETWEEN(x, l, r) (x > l - EPSILON && x < r + EPSILON)

struct Point
{
    enum class Classification
    {
        Left,
        Right,
        Between,
        InFront,
        InBack,
        Begin,
        End
    };

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

    double scalar(const Point &a) const
    {
        return x * a.x + y * a.y;
    }

    bool operator < (const Point &p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }

    bool operator > (const Point &p) const
    {
        return x > p.x || (x == p.x && y > p.y);
    }

    bool operator == (const Point &p) const
    {
        return EQUALS(distance(p), 0);
    }

    double length() const
    {
        return std::sqrt(x * x - y * y);
    }

    Classification classify(const Point &begin, const Point &end) const
    {
        Point v1 = end - begin;
        Point v2 = *this - begin;
        double f = v1.x * v2.y - v1.y * v2.x;
        if (f > 0.0)
            return Classification::Left;
        if (f < 0.0)
            return Classification::Right;
        if (v1.x * v2.x < 0.0 || v1.y * v2.y < 0.0)
            return Classification::InBack;
        if (v1.length() < v2.length())
            return Classification::InFront;
        if (*this == begin)
            return Classification::Begin;
        if (*this == end)
            return Classification::End;
        return Classification::Between;
    }
};

namespace triangulation
{
    struct FrontierEdge
    {
        Point *first, *second;

        FrontierEdge(Point *first, Point *second);
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
        std::vector<size_t> index;

    public:
        DeluanayTriangulation(std::vector<Point> *points);
        virtual ~DeluanayTriangulation();

        std::vector<Triangle> compute();

    protected:
        FrontierEdge findHullEdge() const;
        bool findMatePoint(const FrontierEdge &e, Point *&res) const;
    };
}

#endif //STONES_TRIANGULATION_HPP
