#pragma once
#include <QLabel>
#include <QRubberBand>
#include <QPoint>
#include <opencv2/core.hpp>

// Displays a cv::Mat inside a scrollable QLabel.
// Also handles mouse interaction for the crop tool (rubber-band selection).
class ImageCanvas : public QLabel
{
    Q_OBJECT

public:
    explicit ImageCanvas(QWidget *parent = nullptr);

    void setImage(const cv::Mat &mat);
    cv::Rect  selectedRect() const; // returns selected region in image coordinates
    void clearSelection();

signals:
    void selectionFinished(cv::Rect roi); // emitted when user finishes drawing a selection

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QRubberBand *m_rubberBand = nullptr;
    QPoint       m_origin;
    cv::Size     m_imageSize; // original image dimensions

    // Map canvas pixel → image pixel
    cv::Point canvasToImage(const QPoint &p) const;
};
