#ifndef PLATA_H
#define PLATA_H

#include <QMainWindow>
#include <QScrollArea>
#include <QVBoxLayout>
#include <mupdf/fitz.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Plata; }
QT_END_NAMESPACE

class Plata : public QMainWindow
{
    Q_OBJECT

public:
    explicit Plata(QWidget *parent = nullptr);
    ~Plata();

private slots:
    void open_document();
    void zoom_in();
    void zoom_out();

private:
    Ui::Plata *ui;

    QScrollArea *m_scrollArea;
    QWidget *m_documentContainer;
    QVBoxLayout *m_layout;

    fz_context *m_ctx = nullptr;
    fz_document *m_current_doc = nullptr;
    int m_current_doc_pages = 0;
    float m_zoom = 100.0f;

    void change_zoom(int value);
    void render_pdf(fz_document *doc);

    void clear_layout();       // Clears all QLabel widgets from layout
    void close_document();     // Closes and frees the current PDF document
};

#endif // PLATA_H
