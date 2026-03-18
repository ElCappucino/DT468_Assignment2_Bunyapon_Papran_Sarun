#include "minesweepertable.h"
#include <time.h>
#include <cassert>
#include <stdlib.h>

MineSweeperTable::MineSweeperTable(int n, int bombNo){

    srand(time(NULL));

    this->rows = n;
    this->cols = n;
    this->bombCount = bombNo;
    
    // init
    board.resize(rows);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i].push_back(std::make_unique<Grid>());
        }
    }

    initialize();

    checkInvariant();
}

int MineSweeperTable::getRows() const{

    return this->rows;
}

int MineSweeperTable::getCols() const{

    return this->cols;
}
  
int MineSweeperTable::getBombCount() const{

    return this->bombCount;
}

bool MineSweeperTable::isGameOver() const{

    return this->gameOver;
}
bool MineSweeperTable::isWin() const{

    return this->win;
}
const std::vector<std::vector<std::unique_ptr<Grid>>>& MineSweeperTable::getBoard() const{

    return this->board;
}
void MineSweeperTable::initialize(){

    // Pre condition
    assert(rows > 0 && cols > 0);
    assert(bombCount > 0 && bombCount < rows * cols);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            board[r][c]->reset();
        }
    }
    gameOver = false;
    win = false;

    placeBombs();
    calculateNeighbors();
    checkInvariant();
}
void MineSweeperTable::reset(){

    initialize();
}
bool MineSweeperTable::isValid(int r, int c) const{

    if (r >= rows || r < 0 || c >= cols || c < 0)
    {
        return false;
    }
    return true;
}
void MineSweeperTable::placeBombs(){

    assert(bombCount > 0);
    int placed = 0;
    while (placed < bombCount) {
        int r = rand() % rows;
        int c = rand() % cols;

        if (!board[r][c]->getHasBomb()) {
            board[r][c]->setBomb(true);
            placed++;
        }
    }
    checkInvariant();
        
}
void MineSweeperTable::calculateNeighbors(){
    
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {

            if (board[r][c]->getHasBomb()) continue;

            int count = 0;
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    if (isValid(r + i, c + j) && board[r + i][c + j]->getHasBomb()) {
                        count++;
                    }
                }
            }
            board[r][c]->setNeighborBombCount(count);
        }
    }
    checkInvariant();
}
void MineSweeperTable::openCell(int r, int c){

    if (!isValid(r, c)) { return; }

    if (gameOver) { return; }

    if (board[r][c]->getIsOpen()) { return; }

    if (board[r][c]->getMarked()) { return; }

    if (board[r][c]->getHasBomb()) {
        gameOver = true;
    } else {
        board[r][c]->setOpen();

        // if neightbor is not bomb, recursive open around the cell
        if (board[r][c]->getNeighborBombCount() == 0) {
            recursiveOpen(r, c);
        }
        checkWin();
    }
    checkInvariant();
}

void MineSweeperTable::recursiveOpen(int r, int c){

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int nr = r + i;
            int nc = c + j;
            
            // not open itself
            if (i == 0 && j == 0) continue;

            // recursive open only if the grid has no bomb in neighbor nor itself
            if (isValid(nr, nc) && !board[nr][nc]->getIsOpen() && !board[nr][nc]->getMarked()) {
                board[nr][nc]->setOpen();
                
                if (board[nr][nc]->getNeighborBombCount() == 0) {
                    recursiveOpen(nr, nc);
                }
            }
        }
    }
    checkInvariant();
}
void MineSweeperTable::toggleMark(int r, int c){

    if (isValid(r, c) && !gameOver && !board[r][c]->getIsOpen()) {
        board[r][c]->setMarked(!board[r][c]->getMarked());
    }
    checkInvariant();
}
void MineSweeperTable::checkWin(){

    int openCount = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c]->getIsOpen()) openCount++;
        }
    }
    if (openCount == (rows * cols) - bombCount) {
        win = true;
        gameOver = true;
    }
    checkInvariant();
}
void MineSweeperTable::checkInvariant() const{

    assert(rows > 0 && cols > 0);
    
    int actualBombCount = 0;
    int openCount = 0;

    // check if the actual bomb count is as same as the bomb count variable
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (board[r][c]->getHasBomb()) {
                actualBombCount++;
            }
            if (board[r][c]->getIsOpen()) {
                openCount++;
            }
        }
    }

    assert(actualBombCount == bombCount);

    // check if the logic is correct during the win state
    if (win) {
        assert(gameOver == true);
        assert(openCount == (rows * cols) - bombCount);
    }
}