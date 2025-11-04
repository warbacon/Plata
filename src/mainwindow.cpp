#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <mupdf/fitz.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Plata");

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QWidget *container = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(container);

    for (int i = 0; i < 10; ++i) {
        QLabel *label = new QLabel(QString("Label %1").arg(i + 1));
        layout->addWidget(label);
    }

    scrollArea->setWidget(container);
    setCentralWidget(scrollArea);
}

MainWindow::~MainWindow() { delete ui; }
