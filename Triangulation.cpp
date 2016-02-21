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
    std::set<FrontierEdge> frontier;

    auto updateFrontier = [&] (Point *begin, Point *end) {
        FrontierEdge e(begin, end);
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
        FrontierEdge e = *frontier.begin();
        frontier.erase(frontier.begin());
        if (findMatePoint(e, point)) {
            updateFrontier(point, e.first);
            updateFrontier(e.second, point);
            triangles.push_back({e.first, e.second, point});
        }
    }
    return triangles;
}

FrontierEdge DeluanayTriangulation::findHullEdge() const
{
    size_t base = 0;
    for (size_t i = 0; i < n; i++) {
        if (m_points->at(i) < m_points->at(base))
            base = i;
    }

    std::swap(m_points->at(0), m_points->at(base));
    size_t leftmost = 1;
    for (size_t i = 2; i < n; i++) {
        Point::Classification c = m_points->at(i).classify(m_points->at(0), m_points->at(leftmost));
        if (c == Point::Classification::Left || c == Point::Classification::Between)
            leftmost = i;
    }

    return FrontierEdge(&m_points->at(0), &m_points->at(leftmost));
}

bool DeluanayTriangulation::findMatePoint(const FrontierEdge &e, Point *&res) const
{
    Point *best_point = nullptr;
    double best_value = std::numeric_limits<double>::max();
    double value = 0;

    Segment inv = {*e.first, *e.second};
    inv = inv.rotate();
    for (size_t i = 0; i < n; i++) {
        if (m_points->at(i).classify(*e.first, *e.second) == Point::Classification::Right) {
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

bool Segment::intersect(const Segment &s, double &t) const
{
    Point inv = (s.end - s.begin).inv();
    double scalar = inv.scalar(end - begin);
    if (EQUALS(scalar, 0)) {
        return false;
    }

    double num = inv.scalar(begin - s.begin);
    t = -num / scalar;
    return true;
}

Segment Segment::rotate() const
{
    Point inv = (begin - end).inv();
    Point center = (begin + end) * 0.5;
    return {center - inv * 0.5, center + inv * 0.5};
}

//
// FrontierEdge
//

FrontierEdge::FrontierEdge(Point *first, Point *second)
        : first(first), second(second)
{
}

void FrontierEdge::flip()
{
    std::swap(first, second);
}

bool FrontierEdge::operator<(const FrontierEdge &e) const
{
    return std::make_pair(*first, *second) < std::make_pair(*e.first, *e.second);
}
