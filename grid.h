#ifndef GRID_H
#define GRID_H

/**
 * @class Grid
 * @brief A class that represent grid on the Minesweeper board.
 * 
 * * Invariants:
 * 
 * - neighbor_bomb_count must be between 0 and 8.
 * 
 * - A cell cannot be both open and marked at the same time.
 */
class Grid {
 private:
  bool hasBomb;
  bool isOpen;
  bool marked;
  int neighborBombCount;

 public:
  /**
   * @brief Constructor for a new Grid cell.
   * @post hasBomb == false, isOpen == false, marked == false
   */
  Grid();

  /** @brief Returns if cell has a bomb. */
  bool getHasBomb() const;

  /** @brief Returns if cell is revealed. */
  bool getIsOpen() const;

  /** @brief Returns if cell is flagged. */
  bool getMarked() const;

  /** @brief Returns the neighbor count. */
  int getNeighborBombCount() const;

  /**
   * @brief Sets whether this cell contains a bomb.
   * @param b True if cell is a bomb. False if cell doesn't have bomb.
   */
  void setBomb(bool b);
  
  /**
   * @brief Reveals the cell value.
   * @pre isOpen == false && marked == false
   */
  void setOpen();

  /**
   * @brief Toggles or sets the flag on a cell.
   * @pre isOpen == false
   * @param m True to mark, false to unmark.
   */
  void setMarked(bool m);

  /**
   * @brief Sets the number of adjacent bombs.
   * @pre n >= 0 && n <= 8
   * @param count The number of neighbor bombs.
   */
  void setNeighborBombCount(int n);

  /**
   * @brief Restores the cell to its initial state.
   * @post hasBomb == false, isOpen == false, marked == false, neighborBombCount == 0
   */
  void reset();

 private:

 /**
   * @brief Verifies class invariants.
   * @invariant neighborBombCount >= 0 && neighborBombCount <= 8
   * @invariant if (isOpen), marked == false
   */
  void checkInvariant() const;
};

#endif