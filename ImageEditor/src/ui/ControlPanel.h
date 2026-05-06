#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

// Right-side panel that shows controls relevant to the currently selected feature.
// Each feature gets its own sub-widget inside the QStackedWidget.
class ControlPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ControlPanel(QWidget *parent = nullptr);

    // Called by MainWindow to switch the visible controls
    void showControls(int featureIndex);

signals:
    // ── Core features ──────────────────────────────────────────────
    void applyThreshold(int mode, int value, int blockSize, int C);
    void applyHistEq(int mode, double clipLimit, int tileSize);
    void applyMorphology(int op, int kernelSize, int kernelShape);
    void applyCanny(double t1, double t2, int aperture);
    void applyAffine();        // affine: uses point picker on canvas
    void applyPerspective();   // perspective: uses point picker on canvas
    void applyStitch();        // opens multi-file dialog

    // ── Advanced features ──────────────────────────────────────────
    void applyCartoon();
    void applyPencilSketch(bool color);
    void applyCrop();          // uses rubber-band selection on canvas

private:
    QStackedWidget *m_stack;

    // ── per-feature sub-widgets ─────────────────────────────────────
    QWidget *buildThresholdPanel();
    QWidget *buildHistEqPanel();
    QWidget *buildMorphologyPanel();
    QWidget *buildCannyPanel();
    QWidget *buildGeometricPanel();
    QWidget *buildStitchPanel();
    QWidget *buildCartoonPanel();
    QWidget *buildSketchPanel();
    QWidget *buildCropPanel();

    // helpers
    static QSlider *makeSlider(int min, int max, int val, Qt::Orientation o = Qt::Horizontal);
};
