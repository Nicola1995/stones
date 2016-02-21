#include <iostream>
#include "Triangulation.hpp"
#include <vector>

using namespace std;

int main()
{
    vector<Point> points = {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {2, 2}, {-1, 2}};
    triangulation::DeluanayTriangulation deluanayTriangulation(&points);
    vector<triangulation::Triangle> triangles = deluanayTriangulation.compute();
    for (auto triangle : triangles) {
        cout << "triangle : " << endl;
        cout << triangle.a->x << " " << triangle.a->y << endl;
        cout << triangle.b->x << " " << triangle.b->y << endl;
        cout << triangle.c->x << " " << triangle.c->y << endl;
    }
    return 0;
}