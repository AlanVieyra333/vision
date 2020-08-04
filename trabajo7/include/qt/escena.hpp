#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QWidget>
#include <QtOpenGL>
#include <QTimer>
#include <qpainter.h>
#include <ctype.h>
#include "opencv2/opencv.hpp"

using namespace cv;

class Escena : public QGLWidget {
private:
    //Opencv frames
    VideoCapture *cap;
    Mat frame;
    Mat grayFrame;
    Q_OBJECT // must include this if you use Qt signals/slots
public:
    Escena(QWidget *parent = 0);
    QSizePolicy sizePolicy() const;

    void getPosition(int *x, int *y) {
        *x = xmouse;
        *y = ymouse;
    }

    void getSize(int *w, int *h) {
        *w = windowWidth;
        *h = windowHeight;
    }
    void dibuja_fondo();
public slots:
    void timerDone(void);

    void initscreen(void) {
        // timer->start(41);
        timer->start(33);
    }

    void stop(void) {
        timer->stop();
    }
	void capture(void);

signals:
    void changePos(); // called when mouse is moved
    void changeSize(); // called when windows is resized

protected:
    void initializeGL(void);
    void resizeGL(int w, int h);
    void paintGL(void);
    void mouseMoveEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent * e);

private:
    QTimer *timer;
    int windowWidth, windowHeight; // canvas width and height
    float yrot, xrot; // Encene rotation
    int xmouse, ymouse; // Mouse pointer coordinates
};


#endif // CUBO_H
