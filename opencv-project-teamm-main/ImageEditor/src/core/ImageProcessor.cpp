#include "ImageProcessor.h"
#include <opencv2/stitching.hpp>
#include <QImage>

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

static cv::Mat toGray(const cv::Mat &src)
{
    cv::Mat gray;
    if (src.channels() == 1)
        gray = src.clone();
    else
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    return gray;
}

// 1. Thresholding

cv::Mat ImageProcessor::thresholdBinary(const cv::Mat &src, int thresh)
{
    cv::Mat gray = toGray(src);
    cv::Mat dst;
    cv::threshold(gray, dst, thresh, 255, cv::THRESH_BINARY);
    return dst;
}

cv::Mat ImageProcessor::thresholdOtsu(const cv::Mat &src)
{
    cv::Mat gray = toGray(src);
    cv::Mat dst;
    cv::threshold(gray, dst, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    return dst;
}

cv::Mat ImageProcessor::thresholdAdaptive(const cv::Mat &src, int blockSize, int C)
{
    // blockSize must be odd and >= 3
    if (blockSize % 2 == 0) blockSize++;
    if (blockSize < 3) blockSize = 3;

    cv::Mat gray = toGray(src);
    cv::Mat dst;
    cv::adaptiveThreshold(gray, dst, 255,
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C,
                          cv::THRESH_BINARY, blockSize, C);
    return dst;
}

// 2. Histogram equalization

cv::Mat ImageProcessor::equalizeHistGlobal(const cv::Mat &src)
{
    if (src.channels() == 1)
    {
        cv::Mat dst;
        cv::equalizeHist(src, dst);
        return dst;
    }
    // Color: equalize the Y channel in YCrCb
    cv::Mat ycrcb;
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
    std::vector<cv::Mat> channels;
    cv::split(ycrcb, channels);
    cv::equalizeHist(channels[0], channels[0]);
    cv::merge(channels, ycrcb);
    cv::Mat dst;
    cv::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);
    return dst;
}

cv::Mat ImageProcessor::equalizeHistCLAHE(const cv::Mat &src, double clipLimit, int tileSize)
{
    auto clahe = cv::createCLAHE(clipLimit, cv::Size(tileSize, tileSize));

    if (src.channels() == 1)
    {
        cv::Mat dst;
        clahe->apply(src, dst);
        return dst;
    }
    cv::Mat ycrcb;
    cv::cvtColor(src, ycrcb, cv::COLOR_BGR2YCrCb);
    std::vector<cv::Mat> channels;
    cv::split(ycrcb, channels);
    clahe->apply(channels[0], channels[0]);
    cv::merge(channels, ycrcb);
    cv::Mat dst;
    cv::cvtColor(ycrcb, dst, cv::COLOR_YCrCb2BGR);
    return dst;
}

// 3. Morphology

cv::Mat ImageProcessor::morphologyOp(const cv::Mat &src, int op, int kernelSize, int kernelShape)
{
    // op:          cv::MORPH_DILATE, MORPH_ERODE, MORPH_OPEN, MORPH_CLOSE, MORPH_GRADIENT
    // kernelShape: cv::MORPH_RECT, MORPH_ELLIPSE, MORPH_CROSS
    cv::Mat kernel = cv::getStructuringElement(
        kernelShape,
        cv::Size(kernelSize, kernelSize));
    cv::Mat dst;
    cv::morphologyEx(src, dst, op, kernel);
    return dst;
}

// 4. Canny edge detection

cv::Mat ImageProcessor::cannyEdge(const cv::Mat &src, double t1, double t2, int apertureSize)
{
    cv::Mat gray = toGray(src);
    cv::Mat dst;
    cv::Canny(gray, dst, t1, t2, apertureSize);
    return dst;
}

// 5. Geometric transforms

cv::Mat ImageProcessor::affineTransform(const cv::Mat &src,
                                         cv::Point2f src1, cv::Point2f src2, cv::Point2f src3,
                                         cv::Point2f dst1, cv::Point2f dst2, cv::Point2f dst3)
{
    cv::Point2f srcTri[3] = {src1, src2, src3};
    cv::Point2f dstTri[3] = {dst1, dst2, dst3};
    cv::Mat M = cv::getAffineTransform(srcTri, dstTri);
    cv::Mat dst;
    cv::warpAffine(src, dst, M, src.size());
    return dst;
}

cv::Mat ImageProcessor::perspectiveTransform(const cv::Mat &src,
                                               std::vector<cv::Point2f> srcPts,
                                               std::vector<cv::Point2f> dstPts)
{
    cv::Mat M = cv::getPerspectiveTransform(srcPts, dstPts);
    cv::Mat dst;
    cv::warpPerspective(src, dst, M, src.size());
    return dst;
}


// 6. Panorama stitching

cv::Mat ImageProcessor::stitchImages(const std::vector<cv::Mat> &images)
{
    if (images.size() < 2)
        return images.empty() ? cv::Mat() : images[0];

    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Mat pano;
    cv::Stitcher::Status status = stitcher->stitch(images, pano);

    if (status != cv::Stitcher::OK)
    {
        // Return first image with an error overlay so the GUI can show something
        cv::Mat err = images[0].clone();
        cv::putText(err, "Stitching failed (not enough overlap?)",
                    {10, 30}, cv::FONT_HERSHEY_SIMPLEX, 0.8, {0, 0, 255}, 2);
        return err;
    }
    return pano;
}

// Advanced — Cartoon effect


cv::Mat ImageProcessor::cartoonEffect(const cv::Mat &src)
{
    // 1. Smooth the image with a bilateral filter (preserves edges)
    cv::Mat color;
    for (int i = 0; i < 4; i++)
        cv::bilateralFilter(i == 0 ? src : color, color, 9, 75, 75);

    // 2. Detect edges on the grayscale version
    cv::Mat gray = toGray(src);
    cv::Mat blur, edges;
    cv::medianBlur(gray, blur, 7);
    cv::adaptiveThreshold(blur, edges, 255,
                          cv::ADAPTIVE_THRESH_MEAN_C,
                          cv::THRESH_BINARY, 9, 2);

    // 3. Convert edges to 3-channel and AND with the smoothed color image
    cv::Mat edgesColor;
    cv::cvtColor(edges, edgesColor, cv::COLOR_GRAY2BGR);
    cv::Mat cartoon;
    cv::bitwise_and(color, edgesColor, cartoon);
    return cartoon;
}

// ─────────────────────────────────────────────────────────────────────────────
// Advanced — Pencil sketch
// ─────────────────────────────────────────────────────────────────────────────

cv::Mat ImageProcessor::pencilSketch(const cv::Mat &src, bool color)
{
    cv::Mat gray_sketch, color_sketch;
    cv::pencilSketch(src, gray_sketch, color_sketch,
                     /*sigma_s=*/60, /*sigma_r=*/0.07f, /*shade_factor=*/0.05f);
    return color ? color_sketch : gray_sketch;
}

// ─────────────────────────────────────────────────────────────────────────────
// Advanced — Crop
// ─────────────────────────────────────────────────────────────────────────────

cv::Mat ImageProcessor::cropImage(const cv::Mat &src, const cv::Rect &roi)
{
    cv::Rect safeRoi = roi & cv::Rect(0, 0, src.cols, src.rows);
    if (safeRoi.empty()) return src;
    return src(safeRoi).clone();
}

// ─────────────────────────────────────────────────────────────────────────────
// Utility — cv::Mat → QImage
// ─────────────────────────────────────────────────────────────────────────────

QImage ImageProcessor::matToQImage(const cv::Mat &mat)
{
    if (mat.empty()) return {};

    if (mat.type() == CV_8UC1)
    {
        return QImage(mat.data, mat.cols, mat.rows,
                      (int)mat.step, QImage::Format_Grayscale8).copy();
    }
    if (mat.type() == CV_8UC3)
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows,
                      (int)rgb.step, QImage::Format_RGB888).copy();
    }
    // fallback: convert to 8UC3
    cv::Mat tmp;
    mat.convertTo(tmp, CV_8UC3);
    return matToQImage(tmp);
}
