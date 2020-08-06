#include "qt/escena.hpp"

#include <chrono>

#include "image_classifier.hpp"
#include "viewer/draw_object.hpp"

using namespace std::chrono; 

Escena::Escena(QWidget *parent) : QGLWidget(parent) {
  int cols = 640;
  int rows = 480;
  int deviceID = 0;  // 0 = open default camera
  int apiID = cv::CAP_ANY;

  setFocusPolicy(Qt::StrongFocus);
  timer = new QTimer(this);

  connect(timer, SIGNAL(timeout()), this, SLOT(timerDone()));

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // Initiating camera
  cap = new VideoCapture();
  cap->open(deviceID + apiID);  // open the  camera

  if (!cap->isOpened()) {  // check if we succeeded
    printf("Error while opening camera device cap(1)\n");
    exit(0);
  }

  cap->set(CAP_PROP_FRAME_WIDTH, cols);
  cap->set(CAP_PROP_FRAME_HEIGHT, rows);

  *(cap) >> frame;  // get a new frame from camera

  cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

  xrot = yrot = 0;
}

void Escena::timerDone() {
  this->yrot += 5.0;
  if (this->yrot > 360.0) this->yrot = 0.0;

  updateGL();
}

void Escena::capture() {
  flip(frame, frame, 0);
  imwrite("imagen.png", frame);
}

void Escena::dibuja_fondo() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // getting image from camera
  *(cap) >> frame;  // get a new frame from camera

  flip(frame, frame, 0);
  cv::cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

  // glPushMatrix();
  glDisable(GL_DEPTH_TEST);
  glDrawPixels(frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
  glEnable(GL_DEPTH_TEST);
  // glPopMatrix();

  bool marker_found = marker_recognition(grayFrame, points);

  // if (!marker_found) {
  //   // printf("Marcador no encontrado.\n");
  //   if (are_there_points_prev > 0) {
  //     are_there_points_prev++;
  //     /* Draw objects. */
  //     glPushMatrix();
  //     set_homography(points_prev, frame.rows, frame.cols);
  //     draw_square();
  //     // draw_triangle(this->yrot);
  //     animation_tetraedro(this->yrot);
  //     glPopMatrix();

  //     if (are_there_points_prev > 10) {
  //       are_there_points_prev = 0;
  //     }
  //   }
  // } else {
  if (marker_found) {
    // are_there_points_prev = 1;
    // points_prev[0][0] = points[0][0];
    // points_prev[0][1] = points[0][1];
    // points_prev[1][0] = points[1][0];
    // points_prev[1][1] = points[1][1];
    // points_prev[2][0] = points[2][0];
    // points_prev[2][1] = points[2][1];
    // points_prev[3][0] = points[3][0];
    // points_prev[3][1] = points[3][1];
    // points_prev[4][0] = points[4][0];
    // points_prev[4][1] = points[4][1];
    // printf("Marcador encontrado en los siguientes puntos:\n");
    // for (int j = 0; j < 5; j++) {
    //   printf("%lf %lf\n", points[j].x, points[j].y);
    // }

    /* Draw objects. */
    glPushMatrix();
    set_homography(points, frame.rows, frame.cols);
    draw_square();
    // draw_triangle(this->yrot);
    animation_tetraedro(this->yrot);
    glPopMatrix();
  }
}

void Escena::mouseMoveEvent(QMouseEvent *e) {
  // Not implemented
  return;
}

void Escena::keyPressEvent(QKeyEvent *e) {
  // Not implemented
  return;
}

//
// This method puts the initial coordinates each time window is resized
//

void Escena::resizeGL(int w, int h) {
  windowWidth = w;
  windowHeight = h;

  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, w, 0.0, h, 1.0, 51.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity(); /* clear the matrix */

  emit changeSize();
}

QSizePolicy Escena::sizePolicy() const {
  return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Escena::paintGL(void) {
  //auto start = high_resolution_clock::now();
  dibuja_fondo();
  //auto stop = high_resolution_clock::now();
  //auto duration = duration_cast<microseconds>(stop - start);
  //printf("Duration: %d ms\n", duration.count());
}

void Escena::initializeGL() {
  glClearColor(0.8, 0.8, 0.8, 0.0);  // Background to a grey tone
  // glShadeModel (GL_FLAT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_LINE_SMOOTH);
}
