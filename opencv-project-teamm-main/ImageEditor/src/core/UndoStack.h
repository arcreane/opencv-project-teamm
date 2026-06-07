#pragma once
#include <vector>
#include <opencv2/core.hpp>

// Stores a history of cv::Mat states for undo/redo.
// Each call to push() saves a deep copy of the current image.
class UndoStack
{
public:
    explicit UndoStack(int maxSize = 20);

    void    push(const cv::Mat &state);
    cv::Mat undo(const cv::Mat &current);
    cv::Mat redo(const cv::Mat &current);

    bool canUndo() const;
    bool canRedo() const;
    void clear();

private:
    std::vector<cv::Mat> m_past;    // states before current
    std::vector<cv::Mat> m_future;  // states after current (for redo)
    int m_maxSize;
};
