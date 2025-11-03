#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QScrollArea>
#include <qnamespace.h>
#include <qscrollarea.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Plata");
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setCentralWidget(scrollArea);
}

MainWindow::~MainWindow() { delete ui; }
