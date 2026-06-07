#include "UndoStack.h"

UndoStack::UndoStack(int maxSize)
    : m_maxSize(maxSize)
{}

void UndoStack::push(const cv::Mat &state)
{
    // A new action clears the redo history
    m_future.clear();

    m_past.push_back(state.clone());

    // Keep stack bounded
    if ((int)m_past.size() > m_maxSize)
        m_past.erase(m_past.begin());
}

cv::Mat UndoStack::undo(const cv::Mat &current)
{
    if (m_past.empty())
        return current;

    m_future.push_back(current.clone());
    cv::Mat prev = m_past.back();
    m_past.pop_back();
    return prev;
}

cv::Mat UndoStack::redo(const cv::Mat &current)
{
    if (m_future.empty())
        return current;

    m_past.push_back(current.clone());
    cv::Mat next = m_future.back();
    m_future.pop_back();
    return next;
}

bool UndoStack::canUndo() const { return !m_past.empty(); }
bool UndoStack::canRedo() const { return !m_future.empty(); }

void UndoStack::clear()
{
    m_past.clear();
    m_future.clear();
}
