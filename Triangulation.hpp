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
    double x, y;

    Point operator - (const Point &p) const
    {
        return {x - p.x, y - p.y};
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
        return y < p.y || (EQUALS(y, p.y) && x < p.x);
    }


};

namespace triangulation
{
    enum class Classification
    {
        Left,
        Right,
        Between,
        OutsideLine,
        Error
    };

    struct Edge
    {
        size_t first;
        size_t second;

        void flip()
        {
            std::swap(first, second);
        }

        bool operator<(const Edge &e) const
        {
            return first < e.first || (first == e.first && second < e.second);
        }
    };

    struct Triangle
    {
        int a, b, c;
    };

    class DeluanayTriangulation
    {
    protected:
        size_t n;
        std::vector<Point> *m_points;
        std::vector<size_t> index;

        Edge hullEdge();
        Classification classify(const Point &a, const Point &begin, const Point &end) const;
    public:
        DeluanayTriangulation(std::vector<Point> *points);
        virtual ~DeluanayTriangulation();

        std::vector<Triangle> compute();
    };
}

#endif //STONES_TRIANGULATION_HPP
