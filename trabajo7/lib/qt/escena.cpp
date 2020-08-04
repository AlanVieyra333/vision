#include "qt/escena.hpp"

#include "image_classifier.hpp"
#include "viewer/draw_object.hpp"

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
}

void Escena::timerDone() {
  this->yrot += 4.0;
	if( this->yrot > 360.0 ) this->yrot = 0.0;

  updateGL();
}

void Escena::capture() {
  flip(grayFrame, grayFrame, 0);
  imwrite("imagen.png", grayFrame);
}

void Escena::dibuja_fondo() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glDisable(GL_DEPTH_TEST);

  // getting image from camera
  *(cap) >> frame;  // get a new frame from camera
  // Preprocessing image
  // Thresholding settings
  int threshold_value = 100;
  int threshold_type = 1;
  int const max_BINARY_value = 255;

  flip(frame, frame, 0);
  cv::cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

  glDrawPixels(frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
  glPopMatrix();

  // GaussianBlur(grayFrame, grayFrame, Size(7, 7), 1.5, 1.5); //Smooth filter
  // threshold(grayFrame, grayFrame, threshold_value, max_BINARY_value,
  // threshold_type); bitwise_not(grayFrame, grayFrame);

  // IMG1 is the camera frame in graytones, and can be processed after this line

  // use fast 4-byte alignment (default anyway) if possible
  // glPixelStorei(GL_UNPACK_ALIGNMENT, (grayFrame.step & 3) ? 1 : 4);
  // set length of one complete row in data (doesn't need to equal image.cols)
  // glPixelStorei(GL_UNPACK_ROW_LENGTH, grayFrame.step / grayFrame.elemSize());
  // flip(frame, frame, 0);
  // Drawing color image to the QGL canvas
  //glDrawPixels(frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
  // glDrawPixels(grayFrame.cols, grayFrame.rows, GL_LUMINANCE,
  // GL_UNSIGNED_BYTE, grayFrame.data);

  double points[5][2];  
  bool marker_found = marker_recognition(grayFrame, points);

  if (marker_found) {
    // printf("Marcador encontrado en los siguientes puntos:\n");
    // for (int j = 0; j < 5; j++) {
    //   printf("%lf %lf\n", points[j].x, points[j].y);
    // }

    /* Draw objects. */
    glPushMatrix();
    get_homography(points, frame.rows, frame.cols);
    draw_square();
    draw_triangle(this->yrot);
    glPopMatrix();
  }
  // else {
  //   printf("Marcador no encontrado.\n");
  // }
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
	glLoadIdentity ();			 /* clear the matrix */

  emit changeSize();
}

QSizePolicy Escena::sizePolicy() const {
  return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Escena::paintGL(void) { dibuja_fondo(); }

void Escena::initializeGL() {
  this->yrot = 0;

  glClearColor (0.5, 0.5, 0.5, 0.0);
  glShadeModel (GL_FLAT);

  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();
}
