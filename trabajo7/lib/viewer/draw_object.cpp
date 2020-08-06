#include "viewer/draw_object.hpp"

#include <QtOpenGL>

#include "homography/homografia.h"

double Model[5][2] = {
    {-1., -1.}, {1., -1.}, {0., -0.571428}, {1., 1.}, {-1., 1.}};
double NormalModel[5][2] = {{-1.118034, -0.959426},
                            {1.118034, -0.959426},
                            {0.000000, -0.495188},
                            {1.118034, 1.207020},
                            {-1.118034, 1.207020}};

double MatrixK[16];
double MatrixRt[16];

float tetraedro1[4][3];
float tetraedro2[4][3];
float d;

void set_homography(double Points[5][2], size_t rows, size_t cols) {
  if (homografia(NormalModel, Points, MatrixK, MatrixRt, (double)rows,
                 (double)cols) == 1) {
    glMultMatrixd(MatrixK);
    glMultMatrixd(MatrixRt);
  }
}

void draw_square() {
  glLineWidth(3);

  glBegin(GL_LINE_LOOP);
  glColor3ub(0, 255, 0);
  glVertex3f(Model[0][0], Model[0][1], 0);
  glVertex3f(Model[1][0], Model[1][1], 0);
  glVertex3f(Model[3][0], Model[3][1], 0);
  glVertex3f(Model[4][0], Model[4][1], 0);
  glEnd();
}

void draw_triangle(float rotate_y) {
  //glRotatef(rotate_y, 0.0, 0.0, 1.0);

  glBegin(GL_TRIANGLES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0);
  glVertex3f(0.428571, 0.0, 1.0);
  glEnd();
}

void animation_tetraedro(float angle) {
  static bool tetraedro_initialized = false;

  if (!tetraedro_initialized) {
    tetraedro_initialized = true;

    float l = 2.5;
    d = 0.82916 * l;
    init_tetraedro(tetraedro1, l);
    init_tetraedro(tetraedro2, l);
  }

  glTranslatef(0.0, 0.0, 1.65832);

  glPushMatrix();
  glRotatef(angle, 0.0, 0.0, 1.0);
  glRotatef(180, 0.0, 1.0, 0.0);
  glTranslatef(0.0, 0.0, -d);
  draw_tetraedro(tetraedro2);
  glPopMatrix();

  glTranslatef(0.0, 0.0, -d);
  draw_tetraedro(tetraedro1);
}

void init_tetraedro(float v[][3], float l) {
  float b = 0.61602 * l;

  v[0][0] = 0.0;
  v[0][1] = b;
  v[0][2] = 0.0;

  v[1][0] = -l / 2.0;
  v[1][1] = -l / 4.0;
  v[1][2] = 0.0;

  v[2][0] = l / 2.0;
  v[2][1] = -l / 4.0;
  v[2][2] = 0.0;

  v[3][0] = 0.0;
  v[3][1] = 0.0;
  v[3][2] = d;
}

void draw_tetraedro(float Lv[][3]) {
  static int Lc[4][3] = {{0, 1, 3}, {1, 2, 3}, {2, 0, 3}, {0, 2, 1}};
  // static float Color[4][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0,
  // 0.0, 1.0}, {1.0, 1.0, 1.0} };

  // Para cada cara del tetraedro
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < 3; i++) {
    glColor3ub(0, 0, 255);

    // Para cada vértice de la cara
    for (int j = 0; j < 3; j++) {
      if (j == 2) glColor3ub(255, 255, 255);
      glVertex3fv(&Lv[Lc[i][j]][0]);
    }
  }
  glColor3ub(255, 255, 255);
  for (int j = 0; j < 3; j++) {
    glVertex3fv(&Lv[Lc[3][j]][0]);
  }
  glEnd();
}
