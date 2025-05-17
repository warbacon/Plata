#include "plata.h"
#include "ui_plata.h"
#include <mupdf/fitz.h>
#include <QScrollArea>
#include <QFileDialog>

Plata::Plata(QWidget *parent) : QMainWindow(parent), ui(new Ui::Plata)
{
    ui->setupUi(this);
    QScrollArea *scrollarea = new QScrollArea();
    this->setCentralWidget(scrollarea);
    connect(ui->actionOpen, &QAction::triggered, this, &Plata::open_document);

    fz_context* ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
}

Plata::~Plata() { delete ui; }

void Plata::open_document() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open PDF", "", "PDF Files (*.pdf);;All Files (*)");
    if (!fileName.isEmpty()) {
        qDebug() << "Archivo seleccionado:" << fileName;
    }
}
