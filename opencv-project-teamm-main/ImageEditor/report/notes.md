# Project Notes (→ Report)

Fill this in as you implement each feature.

## Thresholding
- `cv::threshold` with THRESH_BINARY, THRESH_OTSU
- `cv::adaptiveThreshold` (ADAPTIVE_THRESH_GAUSSIAN_C)
- Why: simple segmentation by intensity

## Histogram Equalization
- `cv::equalizeHist` on the Y channel of YCrCb
- `cv::createCLAHE` for tile-based local equalization
- Why: improves low-contrast images without over-saturating

## Morphology
- `cv::morphologyEx` with MORPH_DILATE/ERODE/OPEN/CLOSE/GRADIENT
- `cv::getStructuringElement` for kernel shape
- Why: noise removal, shape extraction

## Canny Edge Detection
- `cv::Canny(gray, dst, t1, t2, aperture)`
- Exposes both thresholds and aperture to the user
- Why: classic, robust edge detector

## Geometric Transforms
- `cv::getAffineTransform` + `cv::warpAffine` (3 points)
- `cv::getPerspectiveTransform` + `cv::warpPerspective` (4 points)
- Why: deskew, correct perspective distortion

## Panorama Stitching
- `cv::Stitcher::create(PANORAMA)` + `stitch()`
- Handles failure gracefully (error overlay)
- Why: combines overlapping photos into a wide image

## Advanced — Cartoon Effect
- 4× bilateral filter pass (preserves edges, smooths color)
- `cv::adaptiveThreshold` on median-blurred gray → edges
- AND with smoothed image → cartoon look
- Why: visually striking, showcases filter composition

## Advanced — Pencil Sketch
- `cv::pencilSketch()` (photo_realistic module)
- Supports grayscale and color output
- Why: single API call, impressive result, easy to demo

## Advanced — Undo/Redo
- `std::vector<cv::Mat>` stacks (past / future)
- Deep clone on each push; bounded to 30 states
- Why: essential UX for any editor

## Advanced — Crop Tool
- `QRubberBand` on `ImageCanvas` for selection
- Maps canvas pixels → image pixels with scale compensation
- `src(roi).clone()` — zero OpenCV complexity, clean result
- Why: fundamental interactive tool
