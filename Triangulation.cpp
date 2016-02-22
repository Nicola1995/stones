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

std::vector<Triangle> DeluanayTriangulation::compute()
{
    std::vector<Triangle> triangles;
    std::set<FrontierEdge> frontier;

    auto updateFrontier = [&] (Point *begin, Point *end) {
        FrontierEdge e = {begin, end};
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
    size_t base = std::min_element(m_points->begin(), m_points->end()) - m_points->begin();
    size_t leftmost = (base + 1) % n;
    size_t candidate = (leftmost + 1) % n;

    for (size_t i = candidate; i != base; i = (i + 1) % n) {
        Classification c = classify(m_points->at(i), m_points->at(base), m_points->at(leftmost));
        if (c == Classification::Left || c == Classification::Between)
            leftmost = i;
    }

    return {&m_points->at(base), &m_points->at(leftmost)};
}

bool DeluanayTriangulation::findMatePoint(const FrontierEdge &e, Point *&res) const
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
    Point inv = (end - begin).inv();
    Point center = (begin + end) * 0.5;
    return {center - inv * 0.5, center + inv * 0.5};
}

//
// FrontierEdge
//

void FrontierEdge::flip()
{
    std::swap(first, second);
}

bool FrontierEdge::operator<(const FrontierEdge &e) const
{
    return std::make_pair(*second, *first) < std::make_pair(*e.second, *e.first);
}

//
// Classification
//

Classification triangulation::classify(const Point &p, const Point &begin, const Point &end)
{
    Point v1 = end - begin;
    Point v2 = p - begin;
    double f = v1.x * v2.y - v1.y * v2.x;
    if (f > 0.0)
        return Classification::Left;
    if (f < 0.0)
        return Classification::Right;
    if (v1.x * v2.x < 0.0 || v1.y * v2.y < 0.0)
        return Classification::InBack;
    if (v1.length() < v2.length())
        return Classification::InFront;
    if (p == begin)
        return Classification::Begin;
    if (p == end)
        return Classification::End;
    return Classification::Between;
}
