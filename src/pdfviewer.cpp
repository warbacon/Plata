#include "pdfviewer.h"
#include "ui_pdfviewer.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollArea>
#include <QTabBar>
#include <QTabWidget>

PDFViewer::PDFViewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::PDFViewer)
{
    ui->setupUi(this);

    m_tabs = new QTabWidget(this);
    m_tabs->setTabsClosable(true);
    m_tabs->tabBar()->installEventFilter(this);
    connect(m_tabs, &QTabWidget::tabCloseRequested, this, &PDFViewer::close_tab);

    setCentralWidget(m_tabs);

    connect(ui->actionOpen_document, &QAction::triggered, this, &PDFViewer::open_document);
}

void PDFViewer::open_document()
{
    const QString &filename =
        QFileDialog::getOpenFileName(this, "Open PDF - Plata", getenv("HOME"), "PDF Files (*.pdf)");

    if (filename.isEmpty()) {
        return;
    }

    QPixmap pixmap = m_renderer.createPixmap(filename.toUtf8().constData());

    QLabel *label = new QLabel;
    label->setPixmap(pixmap);
    label->setAlignment(Qt::AlignHCenter);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(label);
    scrollArea->setWidgetResizable(true);

    m_tabs->addTab(scrollArea, QFileInfo(filename).fileName());
    m_tabs->setCurrentWidget(scrollArea);
}

void PDFViewer::close_tab(int index)
{
    QWidget *widget = m_tabs->widget(index);
    m_tabs->removeTab(index);
    delete widget;
}

bool PDFViewer::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_tabs->tabBar() && event->type() == QEvent::MouseButtonPress) {
        const QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::MiddleButton) {
            const int index = m_tabs->tabBar()->tabAt(mouseEvent->pos());
            if (index != -1) {
                close_tab(index);
            }
            return true;
        }
    }

    return QMainWindow::eventFilter(watched, event);
}

PDFViewer::~PDFViewer() { delete ui; }
