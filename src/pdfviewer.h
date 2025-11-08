#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include "pdfrenderer.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class PDFViewer;
}
QT_END_NAMESPACE

class QTabWidget;
class QFileInfo;

class PDFViewer : public QMainWindow
{
    Q_OBJECT

public:
    PDFViewer(QWidget *parent = nullptr);
    ~PDFViewer();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void open_document();

private slots:
    void close_tab(int index);

private:
    Ui::PDFViewer *ui;
    PDFRenderer m_renderer;
    QTabWidget *m_tabs;
};

#endif // PDFVIEWER_H
