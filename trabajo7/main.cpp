/**
 * A example for drawing crated primitives
 * Fraga 11/09/2000
 */
#include <QApplication>

#include "qt/painter.hpp"

int main(int argc, char **argv) {
  QApplication a(argc, argv);

  Painter w;

  // w.setGeometry( 0, 0, 800, 600);		//355 );

  w.show();         // Show widget
  return a.exec();  // go!
}
