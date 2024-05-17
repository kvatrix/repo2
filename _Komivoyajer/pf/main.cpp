#include "Graph_.h"

#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    makeGraph();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WinW, WinH);
    glutCreateWindow("Графы");
    glClearColor(0.8, 0.8, 0.8, 0.8);
    WinW = glutGet(GLUT_WINDOW_WIDTH);
    WinH = glutGet(GLUT_WINDOW_HEIGHT);


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mouseMove);
    glutMouseFunc(mouseClick);
    glutMainLoop();
    return 0;
}