#ifndef PDFRENDERER_H
#define PDFRENDERER_H

#include <QPixmap>
#include <mupdf/fitz.h>

class PDFRenderer
{
public:
    PDFRenderer();
    ~PDFRenderer();
    QPixmap createPixmap(float scaleFactor, const char *filename);

private:
    fz_context *m_ctx;
};

#endif // PDFRENDERER_H
