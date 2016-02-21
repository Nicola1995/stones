//
// Created by gdhsnlvr on 20.02.16.
//

#include "Triangulation.hpp"

#define EPSILON (1e-9)
#define EQUALS(a, b) (abs(a - b) < EPSILON)
#define BETWEEN(x, l, r) (x > l - EPSILON && x < r + EPSILON)

using namespace ::triangulation;

DeluanayTriangulation::DeluanayTriangulation(std::vector<Point> *points)
    : m_points(points)
{
    n = m_points->size();
}

DeluanayTriangulation::~DeluanayTriangulation()
{

}

std::vector<Triangle> DeluanayTriangulation::compute()
{
    std::vector<Triangle> triangles;
    std::set<Edge> frontier;

    auto updateFrontier = [&] (Point *begin, Point *end) {
        Edge e(begin, end);
        auto it = frontier.find(e);
        if (it != frontier.end())
            frontier.erase(it);
        else {
            e.flip();
            frontier.insert(e);
        }
    };

    Point *point = nullptr;
    frontier.insert(findHullEdge());
    while (!frontier.empty()) {
        Edge e = *frontier.begin();
        frontier.erase(frontier.begin());
        if (findMatePoint(e, point)) {
            updateFrontier(point, e.first);
            updateFrontier(e.second, point);
            triangles.push_back({e.first, e.second, point});
        }
    }
    return triangles;
}

Edge DeluanayTriangulation::findHullEdge() const
{
    size_t base = 0;
    for (size_t i = 0; i < n; i++) {
        if (m_points->at(i) < m_points->at(base))
            base = i;
    }

    std::swap(m_points->at(0), m_points->at(base));
    size_t leftmost = 1;
    for (size_t i = 2; i < n; i++) {
        Classification classification = classify(m_points->at(i), m_points->at(0), m_points->at(leftmost));
        if (classification == Classification::Left || classification == Classification::Between)
            leftmost = i;
    }

    return Edge(&m_points->at(base), &m_points->at(leftmost));
}

DeluanayTriangulation::Classification DeluanayTriangulation::classify(const Point &a, const Point &begin, const Point &end) const
{
    Point vector = end - begin;
    double f = - vector.y * (a.x - begin.x) + vector.x * (a.y - begin.y);
    if (f < 0) return Classification::Left;
    if (f > 0) return Classification::Right;
    double t;
    if (!EQUALS(vector.x, 0)) {
        t = (a.x - begin.x) / vector.x;
    } else if (!EQUALS(vector.y, 0)) {
        t = (a.y - begin.y) / vector.y;
    } else {
        return Classification::Error;
    }
    if (BETWEEN(t, 0, 1))
        return Classification::Between;
    return Classification::OutsideLine;
}

bool DeluanayTriangulation::findMatePoint(const Edge &e, Point *&res) const
{
    Point *best_point = nullptr;
    double best_value = std::numeric_limits<double>::max();
    double value = 0;

    Segment inv = {*e.first, *e.second};
    inv = inv.rotate();
    for (size_t i = 0; i < n; i++) {
        if (classify(m_points->at(i), *e.first, *e.second) == Classification::Right) {
            Segment seg = {*e.second, m_points->at(i)};
            seg = seg.rotate();
            if (inv.intersect(seg, value) && value < best_value) {
                best_value = value;
                best_point = &m_points->at(i);
            }
        }
    }

    if (best_point != nullptr) {
        res = best_point;
        return true;
    }

    return false;
}

//
// Segment
//

bool Segment::intersect(const Segment &s, double t) const
{
    Point v1 = end - begin;
    Point v2 = s.end - s.begin;
    Point v3 = s.begin - begin;
    double det = v1.x * v2.y - v2.x * v1.y;
    double delta = v3.x * v2.y - v2.x * v3.y;
    if (EQUALS(det, 0))
        return false;
    t = delta / det;
    return true;
}

Segment Segment::rotate() const
{
    Point vector = end - begin;
    Point inv = vector.inv();
    Point center = begin + vector * 0.5;
    Point begin = center - inv * 0.5;
    return {begin, begin + inv};
}

//
// Edge
//

Edge::Edge(Point *first, Point *second)
        : first(first), second(second)
{
}

void Edge::flip()
{
    std::swap(first, second);
}

bool Edge::operator<(const Edge &e) const
{
    if (*first < *e.first)
        return true;
    return *second < *e.second;
}
