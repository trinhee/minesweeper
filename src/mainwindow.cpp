#include "mainwindow.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), gameEnded(false)
{
    // Create a central widget and assign a grid layout.
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(5, 5, 5, 5);

    // Add a Reset button on the top spanning the width.
    QPushButton *resetButton = new QPushButton("Reset", this);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetGame);
    gridLayout->addWidget(resetButton, 0, 0, 1, COLS);

    // Create the grid cells.
    cells.resize(ROWS);
    for (int row = 0; row < ROWS; ++row) {
        cells[row].resize(COLS);
        for (int col = 0; col < COLS; ++col) {
            CellButton *btn = new CellButton(row, col, this);
            cells[row][col] = btn;
            // Note: gridLayout row is offset by 1 (row 0 is used by the Reset button)
            gridLayout->addWidget(btn, row + 1, col);
            connect(btn, &CellButton::leftClicked, this, &MainWindow::handleLeftClick);
            connect(btn, &CellButton::rightClicked, this, &MainWindow::handleRightClick);
        }
    }

    setupBoard();

    setWindowTitle("Minesweeper");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupBoard()
{
    gameEnded = false;
    // Reset every cell to its default state.
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            CellButton *btn = cells[row][col];
            btn->setMine(false);
            btn->setAdjacentMines(0);
            btn->setRevealed(false);
            btn->setFlagged(false);
            btn->setQuestioned(false);
            btn->setText("");
            btn->setEnabled(true);
            btn->setStyleSheet("background-color: lightgray; border: 1px solid gray; font-weight: bold;");
        }
    }
    placeMines();
    calculateAdjacents();
}

void MainWindow::placeMines()
{
    int minesPlaced = 0;
    int totalCells = ROWS * COLS;
    while (minesPlaced < MINES) {
        int index = QRandomGenerator::global()->bounded(totalCells);
        int row = index / COLS;
        int col = index % COLS;
        if (!cells[row][col]->isMine()) {
            cells[row][col]->setMine(true);
            minesPlaced++;
        }
    }
}

void MainWindow::calculateAdjacents()
{
    // For every cell that is not a mine, count how many mines are adjacent.
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            if (cells[row][col]->isMine())
                continue;
            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0)
                        continue;
                    int r = row + dr;
                    int c = col + dc;
                    if (r >= 0 && r < ROWS && c >= 0 && c < COLS && cells[r][c]->isMine())
                        count++;
                }
            }
            cells[row][col]->setAdjacentMines(count);
        }
    }
}

void MainWindow::handleLeftClick(int row, int col)
{
    if (gameEnded)
        return;

    CellButton *cell = cells[row][col];
    if (cell->isRevealed())
        return;

    // Reveal the cell. (If it’s a mine the game will end.)
    revealCell(row, col);

    if (checkWin())
        gameOver(true);
}

void MainWindow::handleRightClick(int row, int col)
{
    if (gameEnded)
        return;

    CellButton *cell = cells[row][col];
    if (cell->isRevealed())
        return;

    // Cycle through the marking states: (none) -> flagged -> questioned -> (none)
    if (!cell->isFlagged() && !cell->isQuestioned()) {
        cell->setFlagged(true);
        cell->setQuestioned(false);
        cell->setText("F");
        cell->setStyleSheet("background-color: yellow; border: 1px solid gray; font-weight: bold;");
    } else if (cell->isFlagged()) {
        cell->setFlagged(false);
        cell->setQuestioned(true);
        cell->setText("?");
        cell->setStyleSheet("background-color: orange; border: 1px solid gray; font-weight: bold;");
    } else if (cell->isQuestioned()) {
        cell->setQuestioned(false);
        cell->setText("");
        cell->setStyleSheet("background-color: lightgray; border: 1px solid gray; font-weight: bold;");
    }
}

void MainWindow::revealCell(int row, int col)
{
    // Bounds-check and avoid re‑revealing cells.
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return;

    CellButton *cell = cells[row][col];
    if (cell->isRevealed())
        return;

    cell->setRevealed(true);

    if (cell->isMine()) {
        // Show mine and end the game.
        cell->setText("M");
        cell->setStyleSheet("background-color: red; border: 1px solid gray; font-weight: bold;");
        gameOver(false);
        return;
    } else {
        int adjacent = cell->adjacentMines();
        if (adjacent > 0) {
            cell->setText(QString::number(adjacent));
        } else {
            cell->setText("");
            // Flood-fill reveal for cells with zero adjacent mines.
            revealAdjacentZeros(row, col);
        }
        cell->setStyleSheet("background-color: white; border: 1px solid gray; font-weight: bold;");
    }
}

void MainWindow::revealAdjacentZeros(int row, int col)
{
    // Reveal all neighboring cells that have not yet been revealed.
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0)
                continue;
            int r = row + dr;
            int c = col + dc;
            if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                CellButton *adjCell = cells[r][c];
                if (!adjCell->isRevealed() && !adjCell->isFlagged())
                    revealCell(r, c);
            }
        }
    }
}

bool MainWindow::checkWin()
{
    // The player wins when all non‑mine cells have been revealed.
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            CellButton *cell = cells[row][col];
            if (!cell->isMine() && !cell->isRevealed())
                return false;
        }
    }
    return true;
}

void MainWindow::gameOver(bool won)
{
    gameEnded = true;
    if (!won) {
        // Reveal all mines.
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                CellButton *cell = cells[row][col];
                if (cell->isMine() && !cell->isRevealed()) {
                    cell->setText("M");
                    cell->setStyleSheet("background-color: red; border: 1px solid gray; font-weight: bold;");
                    cell->setRevealed(true);
                }
            }
        }
        QMessageBox::information(this, "Game Over", "You hit a mine! Game Over.");
    } else {
        QMessageBox::information(this, "Congratulations", "You cleared the board! You win!");
    }
}

void MainWindow::resetGame()
{
    setupBoard();
}
