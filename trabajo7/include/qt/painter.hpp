#include <QPushButton>
#include <QLabel>
#include <QLayout>

#ifndef PAINTER_H
#define PAINTER_H

#include "escena.hpp"


class Painter: public QWidget
{
	Q_OBJECT
public:
    Painter( QWidget *parent=0 );

private slots:
	void newPosition( );
	void newSize( );

private:

	Escena *cubo;	
	QLabel *messpos;
	QLabel *messize;
	QLabel *visorMouse;
	QLabel *visorTama;
	QPushButton *myinit, *stop, *capture;
};

#endif
