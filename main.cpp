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
     * Triangulation
     */
    std::srand(std::time(0));
    points.clear();
    for (int i = 0; i < 1000; i++) {
        points.push_back({std::rand() % 600 - 300, std::rand() % 600 - 300});
    }

    triangulation::DeluanayTriangulation deluanayTriangulation(&points);
    triangles = deluanayTriangulation.compute();

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