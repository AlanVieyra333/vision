#include "qt/painter.hpp"
#include "qt/escena.hpp"

Painter::Painter(QWidget *parent)
: QWidget(parent) {

    QPushButton *quit = new QPushButton(tr("Quit"));
    // quit->setFont(QFont("Times", 14, QFont::Bold));
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

    cubo = new Escena(this);
    connect(cubo, SIGNAL(changePos()), SLOT(newPosition()));
    connect(cubo, SIGNAL(changeSize()), SLOT(newSize()));

    myinit = new QPushButton(tr("Init"));
    connect(myinit, SIGNAL(clicked()), cubo, SLOT(initscreen()));

    stop = new QPushButton(tr("Stop"));
    connect(stop, SIGNAL(clicked()), cubo, SLOT(stop()));

    capture = new QPushButton(tr("Capture"));
    connect(capture, SIGNAL(clicked()), cubo, SLOT(capture()));

    // cubo->resize(640,480);
    cubo->setMinimumSize(640, 480);
    // cubo->setMaximumSize(640/2,480/2);
    // cubo->setMaximumSize( 640, 480 );

    QGridLayout *grid = new QGridLayout(); //2x2, 5 pixel border
    grid->addWidget(cubo, 0, 1);
    grid->setColumnStretch(1, 10);

    messpos = new QLabel();
    messpos->setMaximumHeight(20);
    messpos->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    // messpos->setBackgroundColor( messpos->colorGroup().base() );
    messpos->setAlignment(Qt::AlignCenter);

    messize = new QLabel();
    messize->setMaximumHeight(20);
    messize->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
    // messize->setBackgroundColor( messpos->colorGroup().base() );
    messize->setAlignment(Qt::AlignCenter);

    // Labels for the new displays...
    visorMouse = new QLabel(this);
    visorMouse->setMaximumHeight(20);
    visorMouse->setText("Mouse:");
    visorMouse->setAlignment(Qt::AlignCenter);

    // Labels for the new displays...
    visorTama = new QLabel(this);
    visorTama->setMaximumHeight(20);
    visorTama->setText("Tamano:");
    visorTama->setAlignment(Qt::AlignCenter);

    QVBoxLayout *leftBox = new QVBoxLayout;
    grid->addLayout(leftBox, 0, 0);
    leftBox->addWidget(quit);
    leftBox->addWidget(myinit);
    leftBox->addWidget(stop);
    leftBox->addWidget(capture);
    leftBox->addWidget(visorMouse);
    leftBox->addWidget(messpos);
    leftBox->addWidget(visorTama);
    leftBox->addWidget(messize);

    setLayout(grid);
}

void Painter::newPosition() {
    int mx, my;

    cubo->getPosition(&mx, &my);

    QString s;
    messpos->setText(s.sprintf("%d,%d", mx, my));
}

void Painter::newSize() {
    int mx, my;

    cubo->getSize(&mx, &my);

    QString s;
    messize->setText(s.sprintf("%d,%d", mx, my));
}

