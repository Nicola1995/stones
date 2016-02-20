//
// Created by gdhsnlvr on 20.02.16.
//

#include "Triangulation.hpp"

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

    auto updateFrontier = [&] (size_t begin, size_t end) {
        Edge e = {begin, end};
        auto it = frontier.find(e);
        if (it != frontier.end())
            frontier.erase(it);
        else {
            e.flip();
            frontier.insert(e);
        }
    };

    frontier.insert(hullEdge());
    while (!frontier.empty()) {
        Edge e = *frontier.begin();
        frontier.erase(frontier.begin());


    }
    return triangles;
}

Edge DeluanayTriangulation::hullEdge()
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

    return {0, leftmost};
}

Classification DeluanayTriangulation::classify(const Point &a, const Point &begin, const Point &end) const
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
