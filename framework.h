#pragma once
#include "domain.h"
#include "game.h"

Game __game__;

void HandleKeyPress(unsigned char key, int x, int y) {
    __game__.MovePlayer(key, x, y);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    __game__.RenderGame();
    glutSwapBuffers();
}

void Update(int value) {
    glutPostRedisplay();
    glutTimerFunc(domain::FPS, Update, 0);  // 16 мс (~60 FPS)
}

void MainLoop(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window::WIDTH, window::HEIGHT);
    glutInitWindowPosition(window::POSITION.x, window::POSITION.y);
    glutCreateWindow("Digger Game");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, window::WIDTH, window::HEIGHT, 0.0);

    glutDisplayFunc(Display);
    glutPostRedisplay();
    glutTimerFunc(domain::FPS, Update, 0);

    glutKeyboardFunc(HandleKeyPress);

    glutMainLoop();
}