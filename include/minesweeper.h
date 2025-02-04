#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <vector>

using namespace std;

class Minesweeper {
    public:
        Minesweeper(int rows, int cols, int mines);
        void displayBoard();

    private:
        int rows, cols, mines;
        vector<vector<char>> board;
};

#endif