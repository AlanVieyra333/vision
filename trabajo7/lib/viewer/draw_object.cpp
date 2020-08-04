#include "viewer/draw_object.hpp"

#include <QtOpenGL>

#include "homography/homografia.h"

double MatrixK[16];
double MatrixRt[16];

void draw_square(double Points[5][2], size_t rows, size_t cols) {
  double Model[5][2] = {
      {-35., -35.}, {35., -35.}, {0., -20.}, {35., 35.}, {-35., 35.}};

  if (homografia(Model, Points, MatrixK, MatrixRt, (double)rows,
                 (double)cols) == 1) {
    glPushMatrix();
    glEnable(GL_DEPTH_TEST);
    glMultMatrixd(MatrixK);
    glMultMatrixd(MatrixRt);

    glBegin(GL_LINE_LOOP);
    glLineWidth(3);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-35.0, -35.0, 0);
    glVertex3f(35.0, -35.0, 0);
    glVertex3f(35.0, 35.0, 0);
    glVertex3f(-35.0, 35.0, 0);
    glEnd();

    glPopMatrix();
  }
}

void draw_triangle() { glMultMatrixd(MatrixRt); }