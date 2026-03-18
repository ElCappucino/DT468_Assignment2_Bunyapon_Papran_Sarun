#include "grid.h"
#include <cassert>
#include <time.h>
#include <stdlib.h>

Grid::Grid(){
    reset();
    assert(!hasBomb && !isOpen && !marked && neighborBombCount == 0);
}

bool Grid::getHasBomb() const{
    return this->hasBomb;
}

bool Grid::getIsOpen() const{
    return this->isOpen;
}

bool Grid::getMarked() const{
    return this->marked;
}

int Grid::getNeighborBombCount() const{
    return this->neighborBombCount;
}

void Grid::setBomb(bool b){
    this->hasBomb = b;

    checkInvariant();
}


void Grid::setOpen(){

    // pre condition
    assert(!isOpen && !marked);

    this->isOpen = true;

    checkInvariant();
}

void Grid::setMarked(bool m){

    // pre condition
    assert(!isOpen);

    this->marked = m;

    checkInvariant();
}


void Grid::setNeighborBombCount(int n){
    // pre condition
    assert(n >= 0 && n <= 8);
    this->neighborBombCount = n;
    checkInvariant();
}

void Grid::reset(){
    this->hasBomb = false;
    this->isOpen = false;
    this->marked = false;
    this->neighborBombCount = 0;
}

void Grid::checkInvariant() const{
    assert(neighborBombCount >= 0 && neighborBombCount <= 8);

    if (isOpen) {
        assert(!marked); 
    }
}