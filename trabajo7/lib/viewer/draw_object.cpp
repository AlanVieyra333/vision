#include "viewer/draw_object.hpp"

#include <QtOpenGL>

#include "homography/homografia.h"

double Model[5][2] = {
    {-35., -35.}, {35., -35.}, {0., -20.}, {35., 35.}, {-35., 35.}};

double MatrixK[16];
double MatrixRt[16];

void get_homography(double Points[5][2], size_t rows, size_t cols) {
  if (homografia(Model, Points, MatrixK, MatrixRt, (double)rows,
                 (double)cols) == 1) {
    glMultMatrixd(MatrixK);
    glMultMatrixd(MatrixRt);
  }
}

void draw_square() {
  glBegin(GL_LINE_LOOP);
  glLineWidth(3);
  glColor3ub(0, 255, 0);
  glVertex3f(Model[0][0], Model[0][1], 0);
  glVertex3f(Model[1][0], Model[1][1], 0);
  glVertex3f(Model[3][0], Model[3][1], 0);
  glVertex3f(Model[4][0], Model[4][1], 0);
  glEnd();
}

void draw_triangle(float rotate_y) {
  glRotatef(rotate_y, 0.0, 0.0, 1.0);

  glBegin(GL_TRIANGLES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(35.0, 0.0, 0.0);
  glVertex3f(15.0, 0.0, 35.0);
  glEnd();
}