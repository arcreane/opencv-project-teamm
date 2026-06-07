#pragma once
#include <QMainWindow>
#include <opencv2/core.hpp>
#include "../core/UndoStack.h"

class ImageCanvas;
class ControlPanel;
class QLabel;
class QScrollArea;
class QActionGroup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // File
    void openImage();
    void saveImage();
    void saveImageAs();

    // Edit
    void undo();
    void redo();

    // Feature selection (toolbar/menu)
    void selectFeature(int index);

    // Handlers wired from ControlPanel signals
    void onApplyThreshold(int mode, int value, int blockSize, int C);
    void onApplyHistEq(int mode, double clipLimit, int tileSize);
    void onApplyMorphology(int op, int kernelSize, int kernelShape);
    void onApplyCanny(double t1, double t2, int aperture);
    void onApplyAffine();
    void onApplyPerspective();
    void onApplyStitch();
    void onApplyCartoon();
    void onApplyPencilSketch(bool color);
    void onApplyCrop();

private:
    void buildMenus();
    void buildToolbar();
    void updateDisplay();
    void setCurrentImage(const cv::Mat &img); // push to undo + refresh canvas
    void updateUndoRedoActions();

    // Widgets
    ImageCanvas  *m_canvas;
    ControlPanel *m_panel;
    QLabel       *m_statusLabel;
    QScrollArea  *m_scroll;

    // Actions
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_saveAction;

    // State
    cv::Mat    m_current;       // image currently displayed
    QString    m_filePath;      // last opened/saved path
    UndoStack  m_undoStack;

    // For point-picking (affine / perspective)
    std::vector<cv::Point2f> m_pickedPoints;
    int m_pointPickMode = 0; // 0=off, 3=affine, 4=perspective
};
