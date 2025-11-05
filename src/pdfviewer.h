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

class PDFViewer : public QMainWindow
{
    Q_OBJECT

public:
    PDFViewer(QWidget *parent = nullptr);
    ~PDFViewer();

private:
    Ui::PDFViewer *ui;
    PDFRenderer m_renderer;
    void open_document();
};

#endif // PDFVIEWER_H
