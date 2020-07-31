#include "escena.hpp"

#include "image_classifier.hpp"

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

void Escena::timerDone() { updateGL(); }

void Escena::capture() {
  flip(grayFrame, grayFrame, 0);
  imwrite("imagen.png", grayFrame);
}

void Escena::dibuja_fondo() {
  glDisable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // getting image from camera
  *(cap) >> frame;  // get a new frame from camera
  // Preprocessing image
  // Thresholding settings
  int threshold_value = 100;
  int threshold_type = 1;
  int const max_BINARY_value = 255;

  flip(frame, frame, 0);
  cv::cvtColor(frame, grayFrame, COLOR_BGR2GRAY);

  DPOINT points[5];
  bool marker_found = marker_recognition(grayFrame, points);

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
  glDrawPixels(frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
  // glDrawPixels(grayFrame.cols, grayFrame.rows, GL_LUMINANCE,
  // GL_UNSIGNED_BYTE, grayFrame.data);

  if (marker_found) {
    // printf("Marcador encontrado en los siguientes puntos:\n");
    // for (int j = 0; j < 5; j++) {
    //   printf("%lf %lf\n", points[j].x, points[j].y);
    // }

    /* Draw square. */
    glColor3ub(0, 255, 0);
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    glVertex2f((points[0].x / (float) frame.cols) * 2.0 - 1.0, ((points[0].y / (float) frame.rows)) * 2.0 - 1.0);
    glVertex2f((points[1].x / (float) frame.cols) * 2.0 - 1.0, ((points[1].y / (float) frame.rows)) * 2.0 - 1.0);
    glVertex2f((points[3].x / (float) frame.cols) * 2.0 - 1.0, ((points[3].y / (float) frame.rows)) * 2.0 - 1.0);
    glVertex2f((points[4].x / (float) frame.cols) * 2.0 - 1.0, ((points[4].y / (float) frame.rows)) * 2.0 - 1.0);
    glVertex2f((points[0].x / (float) frame.cols) * 2.0 - 1.0, ((points[0].y / (float) frame.rows)) * 2.0 - 1.0);
    glEnd();
  }
  // else {
  //   printf("Marcador no encontrado.\n");
  // }

  glEnable(GL_DEPTH_TEST);
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

void Escena::resizeGL(int l, int a) {
  windowWidth = l;
  windowHeight = a;
  glViewport(0, 0, (GLsizei)l, (GLsizei)a);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  emit changeSize();
}

QSizePolicy Escena::sizePolicy() const {
  return QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void Escena::paintGL(void) { dibuja_fondo(); }

void Escena::initializeGL() {
  glClearColor(0.8, 0.8, 0.8, 0.0);  // Background to a grey tone
}
