#include "cellbutton.h"

CellButton::CellButton(int row, int col, QWidget *parent)
    : QPushButton(parent), m_row(row), m_col(col),
      m_isMine(false), m_adjacentMines(0), m_revealed(false),
      m_flagged(false), m_questioned(false)
{
    // Set a fixed size so the grid looks uniform.
    setFixedSize(25, 25);
    // Use a bold font so numbers/letters are visible.
    setStyleSheet("font-weight: bold;");
}

void CellButton::setMine(bool mine)
{
    m_isMine = mine;
}

void CellButton::setAdjacentMines(int count)
{
    m_adjacentMines = count;
}

void CellButton::setRevealed(bool revealed)
{
    m_revealed = revealed;
    if (revealed) {
        setEnabled(false);
    }
}

void CellButton::setFlagged(bool flagged)
{
    m_flagged = flagged;
}

void CellButton::setQuestioned(bool questioned)
{
    m_questioned = questioned;
}

void CellButton::mousePressEvent(QMouseEvent *event)
{
    // We intercept the mouse click so we can emit our custom signals.
    if(event->button() == Qt::LeftButton) {
        emit leftClicked(m_row, m_col);
    } else if(event->button() == Qt::RightButton) {
        emit rightClicked(m_row, m_col);
    }
    // We do not call the base class implementation to avoid default button behavior.
}
