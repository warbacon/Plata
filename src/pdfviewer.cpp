#include "pdfviewer.h"
#include "ui_pdfviewer.h"

#include <QFileDialog>
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

    fz_context *ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_document *doc = NULL;
    int n_pages = 0;

    std::string fileName =
        QFileDialog::getOpenFileName(this, "Open PDF", "", "PDF Files (*.pdf);;All Files (*)")
            .toStdString();

    fz_try(ctx)
    {
        fz_register_document_handlers(ctx);
        doc = fz_open_document(ctx, fileName.c_str());

        n_pages = fz_count_pages(ctx, doc);
    }
    fz_always(ctx) { fz_drop_document(ctx, doc); }
    fz_catch(ctx) { fprintf(stderr, "Error: %s\n", fz_caught_message(ctx)); }

    fz_drop_context(ctx);

    for (int i = 0; i < n_pages; ++i) {
        QLabel *label = new QLabel(QString("Label %1").arg(i + 1));
        layout->addWidget(label);
    }

    scrollArea->setWidget(container);
    setCentralWidget(scrollArea);
}

MainWindow::~MainWindow() { delete ui; }
