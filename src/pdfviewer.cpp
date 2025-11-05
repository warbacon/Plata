#include "pdfviewer.h"
#include "pdfrenderer.h"
#include "ui_pdfviewer.h"

#include <QFileDialog>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <mupdf/fitz.h>

PDFViewer::PDFViewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::PDFViewer)
{
    ui->setupUi(this);
    connect(ui->actionOpen_document, &QAction::triggered, this, &PDFViewer::open_document);
}

void PDFViewer::open_document()
{
    const char *filename =
        QFileDialog::getOpenFileName(this, "Open PDF - Plata", "", "PDF Files (*.pdf)")
            .toUtf8()
            .constData();

    float scaleFactor = devicePixelRatio();

    QPixmap pixmap = m_renderer.createPixmap(scaleFactor, filename);
    pixmap.setDevicePixelRatio(scaleFactor);

    QLabel *label = new QLabel;
    label->setPixmap(pixmap);
    label->setAlignment(Qt::AlignCenter);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(label);
    scrollArea->setWidgetResizable(true);

    setCentralWidget(scrollArea);
}

PDFViewer::~PDFViewer() { delete ui; }
