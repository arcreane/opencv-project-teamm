#pragma once
#include <QImage>
#include <opencv2/opencv.hpp>

// Central hub for all image-processing operations.
// Every function takes a source Mat and returns a processed Mat.
// The caller (MainWindow) owns the undo stack and decides when to push.
class ImageProcessor
{
public:
    // ── Core features ──────────────────────────────────────────────

    // 1. Thresholding
    static cv::Mat thresholdBinary(const cv::Mat &src, int thresh);
    static cv::Mat thresholdOtsu(const cv::Mat &src);
    static cv::Mat thresholdAdaptive(const cv::Mat &src, int blockSize, int C);

    // 2. Histogram equalization
    static cv::Mat equalizeHistGlobal(const cv::Mat &src);
    static cv::Mat equalizeHistCLAHE(const cv::Mat &src, double clipLimit, int tileSize);

    // 3. Morphology
    static cv::Mat morphologyOp(const cv::Mat &src, int op, int kernelSize, int kernelShape);

    // 4. Canny edge detection
    static cv::Mat cannyEdge(const cv::Mat &src, double t1, double t2, int apertureSize);

    // 5. Geometric transforms
    static cv::Mat affineTransform(const cv::Mat &src,
                                   cv::Point2f src1, cv::Point2f src2, cv::Point2f src3,
                                   cv::Point2f dst1, cv::Point2f dst2, cv::Point2f dst3);
    static cv::Mat perspectiveTransform(const cv::Mat &src,
                                        std::vector<cv::Point2f> srcPts,
                                        std::vector<cv::Point2f> dstPts);

    // 6. Panorama stitching
    static cv::Mat stitchImages(const std::vector<cv::Mat> &images);

    // ── Advanced features ──────────────────────────────────────────

    // Creative effects
    static cv::Mat cartoonEffect(const cv::Mat &src);
    static cv::Mat pencilSketch(const cv::Mat &src, bool color = false);

    // Interactive tools (applied region only)
    static cv::Mat cropImage(const cv::Mat &src, const cv::Rect &roi);

    // ── Utility ───────────────────────────────────────────────────
    // Convert BGR cv::Mat → QImage for display
    static QImage matToQImage(const cv::Mat &mat);
};
