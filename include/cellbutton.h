#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#include <QPushButton>
#include <QMouseEvent>

class CellButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CellButton(int row, int col, QWidget *parent = nullptr);

    int row() const { return m_row; }
    int col() const { return m_col; }

    bool isMine() const { return m_isMine; }
    void setMine(bool mine);

    int adjacentMines() const { return m_adjacentMines; }
    void setAdjacentMines(int count);

    bool isRevealed() const { return m_revealed; }
    void setRevealed(bool revealed);

    bool isFlagged() const { return m_flagged; }
    void setFlagged(bool flagged);

    bool isQuestioned() const { return m_questioned; }
    void setQuestioned(bool questioned);

signals:
    void leftClicked(int row, int col);
    void rightClicked(int row, int col);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    int m_row;
    int m_col;
    bool m_isMine;
    int m_adjacentMines;
    bool m_revealed;
    bool m_flagged;
    bool m_questioned;
};

#endif // CELLBUTTON_H
