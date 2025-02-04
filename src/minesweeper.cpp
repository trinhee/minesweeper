#include "minesweeper.h"
#include <iostream>

using namespace std;

Minesweeper::Minesweeper(int r, int c, int m) : rows(r), cols(c), mines(m) {
    generateBoard();
}

void Minesweeper::generateBoard() {
    board = vector<vector<char>>(rows, vector<char>(cols, '-'));
}

void Minesweeper::displayBoard() {
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
}