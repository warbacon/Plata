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

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);

    m_documentContainer = new QWidget(m_scrollArea);
    m_layout = new QVBoxLayout(m_documentContainer);
    m_layout->setAlignment(Qt::AlignHCenter);
    m_documentContainer->setLayout(m_layout);

    m_scrollArea->setWidget(m_documentContainer);
    this->setCentralWidget(m_scrollArea);

    connect(ui->actionOpen, &QAction::triggered, this, &Plata::open_document);
    connect(ui->actionZoom_in, &QAction::triggered, this, &Plata::zoom_in);
    connect(ui->actionZoom_out, &QAction::triggered, this, &Plata::zoom_out);

    m_ctx = fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED);
    if (!m_ctx) {
        qWarning("Can't create MuPDF context");
        return;
    }

    fz_register_document_handlers(m_ctx);
}

Plata::~Plata()
{
    close_document();
    fz_drop_context(m_ctx);
    delete ui;
}

void Plata::change_zoom(int value)
{
    m_zoom += value;
    if (m_current_doc) {
        render_pdf(m_current_doc);
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

// Helper method to clear the m_layout and delete widgets
void Plata::clear_layout()
{
    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        delete child->widget();  // delete QLabel
        delete child;            // delete QLayoutItem
    }
}

// Helper method to close and free the current document if it exists
void Plata::close_document()
{
    if (m_current_doc) {
        fz_drop_document(m_ctx, m_current_doc);
        m_current_doc = nullptr;
    }
}

// Renders the pages of the PDF document to the UI
void Plata::render_pdf(fz_document *doc)
{
    clear_layout();

    fz_try(m_ctx)
    {
        const fz_matrix ctm = fz_scale(m_zoom / 100.0f, m_zoom / 100.0f);

        for (int i = 0; i < m_current_doc_pages; ++i) {
            fz_pixmap *pix = nullptr;

            fz_try(m_ctx)
            {
                pix = fz_new_pixmap_from_page_number(m_ctx, doc, i, ctm, fz_device_rgb(m_ctx), 0);
            }
            fz_catch(m_ctx)
            {
                qWarning("Can't render page %d", i);
                continue;
            }

            QPixmap pixmap = QPixmap::fromImage(QImage(pix->samples, pix->w, pix->h, pix->stride, QImage::Format_RGB888));
            pixmap.setDevicePixelRatio(devicePixelRatio());

            QLabel *pageLabel = new QLabel;
            pageLabel->setPixmap(pixmap);
            pageLabel->adjustSize();

            m_layout->addWidget(pageLabel);
            fz_drop_pixmap(m_ctx, pix);
        }
    }
    fz_catch(m_ctx)
    {
        fz_report_error(m_ctx);
        qWarning("Failed to render PDF document");
    }
}

// Opens a PDF file and renders it
void Plata::open_document()
{
    m_zoom = 100.0f * devicePixelRatio();

    QString fileName = QFileDialog::getOpenFileName(this, "Open PDF", "", "PDF Files (*.pdf);;All Files (*)");
    if (fileName.isEmpty())
        return;

    qDebug() << "Selected file:" << fileName;

    // Free the previously loaded document if any
    close_document();

    fz_try(m_ctx)
    {
        m_current_doc = fz_open_document(m_ctx, fileName.toUtf8().constData());
        m_current_doc_pages = fz_count_pages(m_ctx, m_current_doc);
        render_pdf(m_current_doc);
    }
    fz_catch(m_ctx)
    {
        fz_report_error(m_ctx);
        qWarning("Failed to open document");
        m_current_doc = nullptr;
    }
}
