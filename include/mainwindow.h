#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "cellbutton.h"

class QGridLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLeftClick(int row, int col);
    void handleRightClick(int row, int col);
    void resetGame();

private:
    void setupBoard();
    void placeMines();
    void calculateAdjacents();
    void revealCell(int row, int col);
    void revealAdjacentZeros(int row, int col);
    bool checkWin();
    void gameOver(bool won);

    QWidget *centralWidget;
    QGridLayout *gridLayout;

    static const int COLS = 30;
    static const int ROWS = 16;
    static const int MINES = 99;

    // A 2D QVector holding pointers to the cell buttons.
    QVector<QVector<CellButton*>> cells;
    bool gameEnded;
};

#endif // MAINWINDOW_H
