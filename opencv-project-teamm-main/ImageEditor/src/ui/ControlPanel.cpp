#include "ControlPanel.h"
#include <opencv2/imgproc.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include <QStackedWidget>
#include <QComboBox>
#include <QPushButton>

// ─────────────────────────────────────────────────────────────────────────────

QSlider *ControlPanel::makeSlider(int min, int max, int val, Qt::Orientation o)
{
    auto *s = new QSlider(o);
    s->setRange(min, max);
    s->setValue(val);
    return s;
}

// ─────────────────────────────────────────────────────────────────────────────

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(260);
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(4, 4, 4, 4);

    m_stack = new QStackedWidget(this);
    m_stack->addWidget(buildThresholdPanel());   // index 0
    m_stack->addWidget(buildHistEqPanel());       // index 1
    m_stack->addWidget(buildMorphologyPanel());   // index 2
    m_stack->addWidget(buildCannyPanel());        // index 3
    m_stack->addWidget(buildGeometricPanel());    // index 4
    m_stack->addWidget(buildStitchPanel());       // index 5
    m_stack->addWidget(buildCartoonPanel());      // index 6
    m_stack->addWidget(buildSketchPanel());       // index 7
    m_stack->addWidget(buildCropPanel());         // index 8

    layout->addWidget(m_stack);
    layout->addStretch();
}

void ControlPanel::showControls(int featureIndex)
{
    m_stack->setCurrentIndex(featureIndex);
}

// ─────────────────────────────────────────────────────────────────────────────
// 0 — Threshold
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildThresholdPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);
    vb->setSpacing(8);

    auto *gb = new QGroupBox("Thresholding");
    auto *gbl = new QVBoxLayout(gb);

    auto *modeCombo = new QComboBox;
    modeCombo->addItems({"Binary", "Otsu", "Adaptive"});

    auto *threshLabel = new QLabel("Threshold: 128");
    auto *threshSlider = makeSlider(0, 255, 128);
    connect(threshSlider, &QSlider::valueChanged,
            [threshLabel](int v){ threshLabel->setText(QString("Threshold: %1").arg(v)); });

    auto *blockLabel = new QLabel("Block size: 11");
    auto *blockSlider = makeSlider(3, 51, 11);
    blockSlider->setSingleStep(2);
    connect(blockSlider, &QSlider::valueChanged,
            [blockLabel](int v){ blockLabel->setText(QString("Block size: %1").arg(v)); });

    auto *cLabel = new QLabel("C: 2");
    auto *cSlider = makeSlider(0, 20, 2);
    connect(cSlider, &QSlider::valueChanged,
            [cLabel](int v){ cLabel->setText(QString("C: %1").arg(v)); });

    auto *btn = new QPushButton("Apply");
    connect(btn, &QPushButton::clicked, [=](){
        emit applyThreshold(modeCombo->currentIndex(),
                            threshSlider->value(),
                            blockSlider->value(),
                            cSlider->value());
    });

    gbl->addWidget(new QLabel("Mode:"));
    gbl->addWidget(modeCombo);
    gbl->addWidget(threshLabel);
    gbl->addWidget(threshSlider);
    gbl->addWidget(blockLabel);
    gbl->addWidget(blockSlider);
    gbl->addWidget(cLabel);
    gbl->addWidget(cSlider);
    gbl->addWidget(btn);

    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 1 — Histogram equalization
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildHistEqPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Histogram Equalization");
    auto *gbl = new QVBoxLayout(gb);

    auto *modeCombo = new QComboBox;
    modeCombo->addItems({"Global", "CLAHE"});

    auto *clipLabel = new QLabel("Clip limit: 2.0");
    auto *clipSlider = makeSlider(1, 40, 20); // *0.1 → 2.0
    connect(clipSlider, &QSlider::valueChanged,
            [clipLabel](int v){ clipLabel->setText(QString("Clip limit: %1").arg(v * 0.1, 0, 'f', 1)); });

    auto *tileLabel = new QLabel("Tile size: 8");
    auto *tileSlider = makeSlider(2, 32, 8);
    connect(tileSlider, &QSlider::valueChanged,
            [tileLabel](int v){ tileLabel->setText(QString("Tile size: %1").arg(v)); });

    auto *btn = new QPushButton("Apply");
    connect(btn, &QPushButton::clicked, [=](){
        emit applyHistEq(modeCombo->currentIndex(),
                         clipSlider->value() * 0.1,
                         tileSlider->value());
    });

    gbl->addWidget(new QLabel("Mode:"));
    gbl->addWidget(modeCombo);
    gbl->addWidget(clipLabel);
    gbl->addWidget(clipSlider);
    gbl->addWidget(tileLabel);
    gbl->addWidget(tileSlider);
    gbl->addWidget(btn);

    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 2 — Morphology
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildMorphologyPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Morphology");
    auto *gbl = new QVBoxLayout(gb);

    auto *opCombo = new QComboBox;
    opCombo->addItems({"Dilate", "Erode", "Open", "Close", "Gradient"});

    auto *shapeCombo = new QComboBox;
    shapeCombo->addItems({"Rect", "Cross", "Ellipse"});

    auto *sizeLabel = new QLabel("Kernel size: 3");
    auto *sizeSlider = makeSlider(1, 21, 3);
    sizeSlider->setSingleStep(2);
    connect(sizeSlider, &QSlider::valueChanged,
            [sizeLabel](int v){ sizeLabel->setText(QString("Kernel size: %1").arg(v)); });

    // map combo index → cv::MORPH_* constant
    auto morphOp = [](int idx) {
        const int ops[] = {cv::MORPH_DILATE, cv::MORPH_ERODE,
                           cv::MORPH_OPEN,   cv::MORPH_CLOSE, cv::MORPH_GRADIENT};
        return ops[idx];
    };
    auto morphShape = [](int idx) {
        const int shapes[] = {cv::MORPH_RECT, cv::MORPH_CROSS, cv::MORPH_ELLIPSE};
        return shapes[idx];
    };

    auto *btn = new QPushButton("Apply");
    connect(btn, &QPushButton::clicked, [=](){
        emit applyMorphology(morphOp(opCombo->currentIndex()),
                             sizeSlider->value(),
                             morphShape(shapeCombo->currentIndex()));
    });

    gbl->addWidget(new QLabel("Operation:"));
    gbl->addWidget(opCombo);
    gbl->addWidget(new QLabel("Kernel shape:"));
    gbl->addWidget(shapeCombo);
    gbl->addWidget(sizeLabel);
    gbl->addWidget(sizeSlider);
    gbl->addWidget(btn);

    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 3 — Canny
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildCannyPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Canny Edge Detection");
    auto *gbl = new QVBoxLayout(gb);

    auto *t1Label = new QLabel("Threshold 1: 50");
    auto *t1Slider = makeSlider(0, 500, 50);
    connect(t1Slider, &QSlider::valueChanged,
            [t1Label](int v){ t1Label->setText(QString("Threshold 1: %1").arg(v)); });

    auto *t2Label = new QLabel("Threshold 2: 150");
    auto *t2Slider = makeSlider(0, 500, 150);
    connect(t2Slider, &QSlider::valueChanged,
            [t2Label](int v){ t2Label->setText(QString("Threshold 2: %1").arg(v)); });

    auto *apCombo = new QComboBox;
    apCombo->addItems({"3", "5", "7"});

    auto *btn = new QPushButton("Apply");
    connect(btn, &QPushButton::clicked, [=](){
        emit applyCanny(t1Slider->value(), t2Slider->value(),
                        apCombo->currentText().toInt());
    });

    gbl->addWidget(t1Label); gbl->addWidget(t1Slider);
    gbl->addWidget(t2Label); gbl->addWidget(t2Slider);
    gbl->addWidget(new QLabel("Aperture size:")); gbl->addWidget(apCombo);
    gbl->addWidget(btn);

    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 4 — Geometric transforms
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildGeometricPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Geometric Transforms");
    auto *gbl = new QVBoxLayout(gb);

    gbl->addWidget(new QLabel(
        "Click 3 points on the image\n"
        "for Affine, or 4 points\n"
        "for Perspective."));

    auto *affineBtn = new QPushButton("Apply Affine (3 pts)");
    auto *perspBtn  = new QPushButton("Apply Perspective (4 pts)");
    connect(affineBtn, &QPushButton::clicked, this, &ControlPanel::applyAffine);
    connect(perspBtn,  &QPushButton::clicked, this, &ControlPanel::applyPerspective);

    gbl->addWidget(affineBtn);
    gbl->addWidget(perspBtn);

    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 5 — Stitching
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildStitchPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Panorama Stitching");
    auto *gbl = new QVBoxLayout(gb);

    gbl->addWidget(new QLabel(
        "Opens a file dialog.\n"
        "Select 2+ overlapping images."));

    auto *btn = new QPushButton("Select images & Stitch");
    connect(btn, &QPushButton::clicked, this, &ControlPanel::applyStitch);

    gbl->addWidget(btn);
    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 6 — Cartoon
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildCartoonPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Cartoon Effect");
    auto *gbl = new QVBoxLayout(gb);

    gbl->addWidget(new QLabel(
        "Applies bilateral filtering\n"
        "and adaptive edge overlay."));

    auto *btn = new QPushButton("Apply Cartoon");
    connect(btn, &QPushButton::clicked, this, &ControlPanel::applyCartoon);

    gbl->addWidget(btn);
    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 7 — Pencil sketch
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildSketchPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Pencil Sketch");
    auto *gbl = new QVBoxLayout(gb);

    auto *colorCheck = new QCheckBox("Color sketch");

    auto *btn = new QPushButton("Apply Sketch");
    connect(btn, &QPushButton::clicked, [=](){
        emit applyPencilSketch(colorCheck->isChecked());
    });

    gbl->addWidget(colorCheck);
    gbl->addWidget(btn);
    vb->addWidget(gb);
    return w;
}

// ─────────────────────────────────────────────────────────────────────────────
// 8 — Crop
// ─────────────────────────────────────────────────────────────────────────────

QWidget *ControlPanel::buildCropPanel()
{
    auto *w = new QWidget;
    auto *vb = new QVBoxLayout(w);

    auto *gb = new QGroupBox("Crop Tool");
    auto *gbl = new QVBoxLayout(gb);

    gbl->addWidget(new QLabel(
        "Draw a rectangle on the\n"
        "image, then click Crop."));

    auto *btn = new QPushButton("Crop to selection");
    connect(btn, &QPushButton::clicked, this, &ControlPanel::applyCrop);

    gbl->addWidget(btn);
    vb->addWidget(gb);
    return w;
}
