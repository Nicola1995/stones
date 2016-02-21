#include <iostream>
#include <ctime>
#include <GL/glut.h>

#include "Triangulation.hpp"

std::vector<Point> points;
std::vector<triangulation::Triangle> triangles;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // to right and up or down
    //glutPostRedisplay();

    gluOrtho2D(-300, 300, -300, 300);
    glColor3f(1.0,1.0,1.0);

    // This lines are the two main lines for the coordinate system
    // The crossing point of the line is the (0,0,0)
    // Since its just 2 dimensional z is 0

    for (auto p : points) {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i <= 10; i++) {
            double x = p.x + 3.0 * cos (i * 2.0 * M_PI / 10.0);
            double y = p.y + 3.0 * sin (i * 2.0 * M_PI / 10.0);
            glVertex2d(x, y);
        }
        glEnd();
    }

    glBegin(GL_LINES);
        for (auto triangle : triangles) {
            glVertex2d(triangle.a->x, triangle.a->y);
            glVertex2d(triangle.b->x, triangle.b->y);
            glVertex2d(triangle.b->x, triangle.b->y);
            glVertex2d(triangle.c->x, triangle.c->y);
            glVertex2d(triangle.c->x, triangle.c->y);
            glVertex2d(triangle.a->x, triangle.a->y);
        }
    glEnd();

    // Don't wait start processing buffered OpenGL routines
    glFlush();
}

int main(int argc, char **args)
{
    /*
     * Usage
     */
    triangulation::Segment s1 = {{4, 4}, {6, 6}};
    triangulation::Segment s2 = {{2, 4}, {2, 0}};
    double param = 0.0;
    bool intersect = s1.intersect(s2, param);
    std::cout << intersect << " " << param << std::endl;

    double scalar = (s2.end - s2.begin).inv().scalar(s1.end - s1.begin);
    double num = (s2.end - s2.begin).inv().scalar(s1.begin - s2.begin);
    std::cout << "scalar = " << scalar << std::endl;
    std::cout << "num = " << num << std::endl;

    s2 = s2.rotate();
    std::cout << s2.begin.x << " " << s2.begin.y << std::endl;
    std::cout << s2.end.x << " " << s2.end.y << std::endl;


    /*
     * Triangulation
     */
    std::srand(std::time(0));
    for (int i = 0; i < 100; i++) {
        points.push_back({std::rand() % 600 - 300, std::rand() % 600 - 300});
    }

    triangulation::DeluanayTriangulation deluanayTriangulation(&points);
    triangles = deluanayTriangulation.compute();
    std::cout << triangles.size() << std::endl;
    for (auto triangle : triangles) {
        std::cout << triangle.a->x << " " << triangle.a->y << std::endl;
    }

    /*
     * Simple output
     */
    glutInit(&argc, args);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600,600);  	// Size of the OpenGL window
    glutCreateWindow("Triangulation"); // Creates OpenGL Window
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}