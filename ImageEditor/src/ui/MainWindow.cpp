#include "MainWindow.h"
#include "ImageCanvas.h"
#include "ControlPanel.h"
#include "../core/ImageProcessor.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QScrollArea>
#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QAction>
#include <QActionGroup>
#include <QKeySequence>

#include <opencv2/imgcodecs.hpp>

// Feature names — must match ControlPanel stack indices
static const QStringList FEATURE_NAMES = {
    "Threshold", "Hist. Eq.", "Morphology", "Canny",
    "Geometric", "Stitch",
    "Cartoon", "Sketch", "Crop"
};

// ─────────────────────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_undoStack(30)
{
    setWindowTitle("MyImageEditor");
    resize(1100, 700);

    // Central widget: canvas on the left, panel on the right
    auto *central = new QWidget(this);
    auto *hbox    = new QHBoxLayout(central);
    hbox->setContentsMargins(4, 4, 4, 4);
    hbox->setSpacing(4);

    m_canvas = new ImageCanvas;
    m_scroll = new QScrollArea;
    m_scroll->setWidget(m_canvas);
    m_scroll->setWidgetResizable(true);

    m_panel = new ControlPanel;

    hbox->addWidget(m_scroll, 1);
    hbox->addWidget(m_panel, 0);
    setCentralWidget(central);

    // Status bar
    m_statusLabel = new QLabel("No image loaded.");
    statusBar()->addWidget(m_statusLabel);

    buildMenus();
    buildToolbar();

    // ── Wire ControlPanel signals ──────────────────────────────────
    connect(m_panel, &ControlPanel::applyThreshold,   this, &MainWindow::onApplyThreshold);
    connect(m_panel, &ControlPanel::applyHistEq,      this, &MainWindow::onApplyHistEq);
    connect(m_panel, &ControlPanel::applyMorphology,  this, &MainWindow::onApplyMorphology);
    connect(m_panel, &ControlPanel::applyCanny,       this, &MainWindow::onApplyCanny);
    connect(m_panel, &ControlPanel::applyAffine,      this, &MainWindow::onApplyAffine);
    connect(m_panel, &ControlPanel::applyPerspective, this, &MainWindow::onApplyPerspective);
    connect(m_panel, &ControlPanel::applyStitch,      this, &MainWindow::onApplyStitch);
    connect(m_panel, &ControlPanel::applyCartoon,     this, &MainWindow::onApplyCartoon);
    connect(m_panel, &ControlPanel::applyPencilSketch,this, &MainWindow::onApplyPencilSketch);
    connect(m_panel, &ControlPanel::applyCrop,        this, &MainWindow::onApplyCrop);

    // ── Wire canvas crop selection ─────────────────────────────────
    connect(m_canvas, &ImageCanvas::selectionFinished, [this](cv::Rect /*roi*/){
        // Selection is stored in canvas; user clicks "Crop" in panel to confirm
    });

    // Start on first feature
    m_panel->showControls(0);
}

// ─────────────────────────────────────────────────────────────────────────────
// UI construction
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::buildMenus()
{
    // File
    auto *fileMenu = menuBar()->addMenu("&File");
    auto *openAct  = fileMenu->addAction("&Open...", this, &MainWindow::openImage, QKeySequence::Open);
    m_saveAction   = fileMenu->addAction("&Save",    this, &MainWindow::saveImage,    QKeySequence::Save);
    fileMenu->addAction("Save &As...", this, &MainWindow::saveImageAs);
    fileMenu->addSeparator();
    fileMenu->addAction("&Quit", qApp, &QApplication::quit, QKeySequence::Quit);
    m_saveAction->setEnabled(false);

    // Edit
    auto *editMenu = menuBar()->addMenu("&Edit");
    m_undoAction = editMenu->addAction("&Undo", this, &MainWindow::undo, QKeySequence::Undo);
    m_redoAction = editMenu->addAction("&Redo", this, &MainWindow::redo, QKeySequence::Redo);
    m_undoAction->setEnabled(false);
    m_redoAction->setEnabled(false);

    // Features
    auto *featMenu   = menuBar()->addMenu("&Features");
    auto *featGroup  = new QActionGroup(this);
    featGroup->setExclusive(true);
    for (int i = 0; i < FEATURE_NAMES.size(); ++i)
    {
        auto *a = featMenu->addAction(FEATURE_NAMES[i]);
        a->setCheckable(true);
        a->setData(i);
        featGroup->addAction(a);
        if (i == 0) a->setChecked(true);
        connect(a, &QAction::triggered, [this, i](){ selectFeature(i); });
        if (i == 5) featMenu->addSeparator(); // separator before advanced
    }
}

void MainWindow::buildToolbar()
{
    auto *tb = addToolBar("Features");
    tb->setToolButtonStyle(Qt::ToolButtonTextOnly);
    for (int i = 0; i < FEATURE_NAMES.size(); ++i)
    {
        auto *btn = tb->addAction(FEATURE_NAMES[i]);
        connect(btn, &QAction::triggered, [this, i](){ selectFeature(i); });
        if (i == 5) tb->addSeparator();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// File operations
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::openImage()
{
    QString path = QFileDialog::getOpenFileName(
        this, "Open Image", {},
        "Images (*.png *.jpg *.jpeg *.bmp *.tiff *.webp);;All files (*)");
    if (path.isEmpty()) return;

    cv::Mat img = cv::imread(path.toStdString());
    if (img.empty())
    {
        QMessageBox::warning(this, "Error", "Could not load image:\n" + path);
        return;
    }
    m_filePath = path;
    m_undoStack.clear();
    m_current = img;
    updateDisplay();
    m_saveAction->setEnabled(true);
    m_statusLabel->setText(QString("%1  |  %2 × %3")
        .arg(QFileInfo(path).fileName())
        .arg(img.cols).arg(img.rows));
}

void MainWindow::saveImage()
{
    if (m_filePath.isEmpty()) { saveImageAs(); return; }
    cv::imwrite(m_filePath.toStdString(), m_current);
    m_statusLabel->setText("Saved: " + m_filePath);
}

void MainWindow::saveImageAs()
{
    QString path = QFileDialog::getSaveFileName(
        this, "Save Image As", {},
        "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp);;All files (*)");
    if (path.isEmpty()) return;
    m_filePath = path;
    saveImage();
}

// ─────────────────────────────────────────────────────────────────────────────
// Edit — Undo / Redo
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::undo()
{
    m_current = m_undoStack.undo(m_current);
    updateDisplay();
    updateUndoRedoActions();
}

void MainWindow::redo()
{
    m_current = m_undoStack.redo(m_current);
    updateDisplay();
    updateUndoRedoActions();
}

void MainWindow::updateUndoRedoActions()
{
    m_undoAction->setEnabled(m_undoStack.canUndo());
    m_redoAction->setEnabled(m_undoStack.canRedo());
}

// ─────────────────────────────────────────────────────────────────────────────
// Feature selection
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::selectFeature(int index)
{
    m_panel->showControls(index);
    m_pickedPoints.clear();
    m_canvas->clearSelection();
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setCurrentImage(const cv::Mat &img)
{
    m_undoStack.push(m_current); // save previous state
    m_current = img;
    updateDisplay();
    updateUndoRedoActions();
}

void MainWindow::updateDisplay()
{
    if (!m_current.empty())
        m_canvas->setImage(m_current);
}

// ─────────────────────────────────────────────────────────────────────────────
// Feature handlers
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::onApplyThreshold(int mode, int value, int blockSize, int C)
{
    if (m_current.empty()) return;
    cv::Mat result;
    switch (mode)
    {
        case 0: result = ImageProcessor::thresholdBinary(m_current, value); break;
        case 1: result = ImageProcessor::thresholdOtsu(m_current); break;
        case 2: result = ImageProcessor::thresholdAdaptive(m_current, blockSize, C); break;
    }
    if (!result.empty()) setCurrentImage(result);
}

void MainWindow::onApplyHistEq(int mode, double clipLimit, int tileSize)
{
    if (m_current.empty()) return;
    cv::Mat result;
    if (mode == 0)
        result = ImageProcessor::equalizeHistGlobal(m_current);
    else
        result = ImageProcessor::equalizeHistCLAHE(m_current, clipLimit, tileSize);
    if (!result.empty()) setCurrentImage(result);
}

void MainWindow::onApplyMorphology(int op, int kernelSize, int kernelShape)
{
    if (m_current.empty()) return;
    setCurrentImage(ImageProcessor::morphologyOp(m_current, op, kernelSize, kernelShape));
}

void MainWindow::onApplyCanny(double t1, double t2, int aperture)
{
    if (m_current.empty()) return;
    setCurrentImage(ImageProcessor::cannyEdge(m_current, t1, t2, aperture));
}

void MainWindow::onApplyAffine()
{
    if (m_pickedPoints.size() < 6)
    {
        QMessageBox::information(this, "Affine Transform",
            "Please click 3 source points then 3 destination points on the image.\n"
            "(Feature not yet fully implemented — add mouse-click picking to ImageCanvas.)");
        return;
    }
    auto result = ImageProcessor::affineTransform(
        m_current,
        m_pickedPoints[0], m_pickedPoints[1], m_pickedPoints[2],
        m_pickedPoints[3], m_pickedPoints[4], m_pickedPoints[5]);
    setCurrentImage(result);
    m_pickedPoints.clear();
}

void MainWindow::onApplyPerspective()
{
    if (m_pickedPoints.size() < 8)
    {
        QMessageBox::information(this, "Perspective Transform",
            "Please click 4 source points then 4 destination points on the image.");
        return;
    }
    std::vector<cv::Point2f> src(m_pickedPoints.begin(), m_pickedPoints.begin() + 4);
    std::vector<cv::Point2f> dst(m_pickedPoints.begin() + 4, m_pickedPoints.begin() + 8);
    setCurrentImage(ImageProcessor::perspectiveTransform(m_current, src, dst));
    m_pickedPoints.clear();
}

void MainWindow::onApplyStitch()
{
    QStringList paths = QFileDialog::getOpenFileNames(
        this, "Select images to stitch", {},
        "Images (*.png *.jpg *.jpeg *.bmp)");
    if (paths.size() < 2)
    {
        QMessageBox::information(this, "Stitching", "Please select at least 2 images.");
        return;
    }
    std::vector<cv::Mat> imgs;
    for (const auto &p : paths)
    {
        cv::Mat m = cv::imread(p.toStdString());
        if (!m.empty()) imgs.push_back(m);
    }
    setCurrentImage(ImageProcessor::stitchImages(imgs));
}

void MainWindow::onApplyCartoon()
{
    if (m_current.empty()) return;
    setCurrentImage(ImageProcessor::cartoonEffect(m_current));
}

void MainWindow::onApplyPencilSketch(bool color)
{
    if (m_current.empty()) return;
    setCurrentImage(ImageProcessor::pencilSketch(m_current, color));
}

void MainWindow::onApplyCrop()
{
    if (m_current.empty()) return;
    cv::Rect roi = m_canvas->selectedRect();
    if (roi.area() < 4)
    {
        QMessageBox::information(this, "Crop", "Draw a selection on the image first.");
        return;
    }
    setCurrentImage(ImageProcessor::cropImage(m_current, roi));
    m_canvas->clearSelection();
}
