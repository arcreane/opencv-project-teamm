#include "ImageCanvas.h"
#include "../core/ImageProcessor.h"
#include <QMouseEvent>
#include <QRubberBand>

ImageCanvas::ImageCanvas(QWidget *parent)
    : QLabel(parent)
{
    setAlignment(Qt::AlignCenter);
    setMinimumSize(400, 400);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("background-color: #2b2b2b;");
    m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

void ImageCanvas::setImage(const cv::Mat &mat)
{
    m_imageSize = {mat.cols, mat.rows};
    QImage img = ImageProcessor::matToQImage(mat);
    setPixmap(QPixmap::fromImage(img).scaled(
        size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

cv::Point ImageCanvas::canvasToImage(const QPoint &p) const
{
    // The pixmap is centered and scaled — find its actual rect inside the label
    if (!pixmap() || pixmap()->isNull()) return {};
    QSize pxSize = pixmap()->size();
    int offX = (width()  - pxSize.width())  / 2;
    int offY = (height() - pxSize.height()) / 2;
    double scaleX = (double)m_imageSize.width  / pxSize.width();
    double scaleY = (double)m_imageSize.height / pxSize.height();
    int imgX = (int)((p.x() - offX) * scaleX);
    int imgY = (int)((p.y() - offY) * scaleY);
    return {imgX, imgY};
}

cv::Rect ImageCanvas::selectedRect() const
{
    if (!m_rubberBand->isVisible()) return {};
    QRect r = m_rubberBand->geometry();
    cv::Point tl = canvasToImage(r.topLeft());
    cv::Point br = canvasToImage(r.bottomRight());
    return cv::Rect(tl, br);
}

void ImageCanvas::clearSelection()
{
    m_rubberBand->hide();
}

void ImageCanvas::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        m_origin = e->pos();
        m_rubberBand->setGeometry(QRect(m_origin, QSize()));
        m_rubberBand->show();
    }
}

void ImageCanvas::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
        m_rubberBand->setGeometry(QRect(m_origin, e->pos()).normalized());
}

void ImageCanvas::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        cv::Rect roi = selectedRect();
        if (roi.area() > 4)
            emit selectionFinished(roi);
    }
}
