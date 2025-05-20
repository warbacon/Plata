#include "plata.h"
#include "ui_plata.h"
#include <mupdf/fitz.h>
#include <QScrollArea>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include <QDebug>
#include <QWheelEvent>

Plata::Plata(QWidget *parent) : QMainWindow(parent), ui(new Ui::Plata)
{
    ui->setupUi(this);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    documentContainer = new QWidget(scrollArea);
    layout = new QVBoxLayout(documentContainer);
    layout->setAlignment(Qt::AlignHCenter);
    documentContainer->setLayout(layout);

    scrollArea->setWidget(documentContainer);
    this->setCentralWidget(scrollArea);

    connect(ui->actionOpen, &QAction::triggered, this, &Plata::open_document);
    connect(ui->actionZoom_in, &QAction::triggered, this, &Plata::zoom_in);
    connect(ui->actionZoom_out, &QAction::triggered, this, &Plata::zoom_out);

    ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
    if (!ctx) {
        qWarning("Can't create MuPDF context");
        return;
    }

    fz_register_document_handlers(ctx);
}

Plata::~Plata()
{
    close_document();
    fz_drop_context(ctx);
    delete ui;
}

void Plata::change_zoom(int value)
{
    zoom += value;
    if (current_doc) {
        render_pdf(current_doc);
    }
}

void Plata::zoom_in()
{
    change_zoom(50);
}

void Plata::zoom_out()
{
    change_zoom(-50);
}

// Helper method to clear the layout and delete widgets
void Plata::clear_layout()
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();  // delete QLabel
        delete child;            // delete QLayoutItem
    }
}

// Helper method to close and free the current document if it exists
void Plata::close_document()
{
    if (current_doc) {
        fz_drop_document(ctx, current_doc);
        current_doc = nullptr;
    }
}

// Renders the pages of the PDF document to the UI
void Plata::render_pdf(fz_document *doc)
{
    clear_layout();

    fz_try(ctx)
    {
        const fz_matrix ctm = fz_scale(zoom / 100.0f, zoom / 100.0f);

        for (int i = 0; i < current_doc_pages; ++i) {
            fz_pixmap *pix = nullptr;

            fz_try(ctx)
            {
                pix = fz_new_pixmap_from_page_number(ctx, doc, i, ctm, fz_device_rgb(ctx), 0);
            }
            fz_catch(ctx)
            {
                qWarning("Can't render page %d", i);
                continue;
            }

            QPixmap pixmap = QPixmap::fromImage(QImage(pix->samples, pix->w, pix->h, pix->stride, QImage::Format_RGB888));
            pixmap.setDevicePixelRatio(devicePixelRatio());

            QLabel *pageLabel = new QLabel;
            pageLabel->setPixmap(pixmap);
            pageLabel->adjustSize();

            layout->addWidget(pageLabel);
            fz_drop_pixmap(ctx, pix);
        }
    }
    fz_catch(ctx)
    {
        fz_report_error(ctx);
        qWarning("Failed to render PDF document");
    }
}

// Opens a PDF file and renders it
void Plata::open_document()
{
    zoom = 100.0f * devicePixelRatio();

    QString fileName = QFileDialog::getOpenFileName(this, "Open PDF", "", "PDF Files (*.pdf);;All Files (*)");
    if (fileName.isEmpty())
        return;

    qDebug() << "Selected file:" << fileName;

    // Free the previously loaded document if any
    close_document();

    fz_try(ctx)
    {
        current_doc = fz_open_document(ctx, fileName.toUtf8().constData());
        current_doc_pages = fz_count_pages(ctx, current_doc);
        render_pdf(current_doc);
    }
    fz_catch(ctx)
    {
        fz_report_error(ctx);
        qWarning("Failed to open document");
        current_doc = nullptr;
    }
}
