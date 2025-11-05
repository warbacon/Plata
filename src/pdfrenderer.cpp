#include "pdfrenderer.h"

#include <QImage>
#include <QPixmap>
#include <mupdf/fitz.h>

PDFRenderer::PDFRenderer() : m_ctx(fz_new_context(nullptr, nullptr, FZ_STORE_UNLIMITED))
{
    fz_register_document_handlers(m_ctx);
}

QPixmap PDFRenderer::createPixmap(float scaleFactor, const char *filename)
{
    QPixmap pixmap;
    fz_pixmap *pix = nullptr;
    fz_document *doc = nullptr;

    fz_try(m_ctx)
    {
        const fz_matrix ctm = fz_scale(scaleFactor * 1.1f, scaleFactor * 1.1f);

        doc = fz_open_document(m_ctx, filename);
        pix = fz_new_pixmap_from_page_number(m_ctx, doc, 0, ctm, fz_device_rgb(m_ctx), 0);

        pixmap = QPixmap::fromImage(
            QImage(pix->samples, pix->w, pix->h, pix->stride, QImage::Format_RGB888));
    }
    fz_always(m_ctx)
    {
        fz_drop_pixmap(m_ctx, pix);
        fz_drop_document(m_ctx, doc);
    }
    fz_catch(m_ctx)
    {
        fz_report_error(m_ctx);
        qWarning("Can't render document %s", filename);
    }

    return pixmap;
}

PDFRenderer::~PDFRenderer() { fz_drop_context(m_ctx); }
