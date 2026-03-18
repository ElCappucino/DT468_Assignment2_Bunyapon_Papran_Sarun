#ifndef MINETABLE_H
#define MINETABLE_H

#include <memory>
#include <vector>

#include "grid.h"

/**
 * @class MineSweeperTable
 * @brief Manages the Minesweeper board logic.
 */
class MineSweeperTable {
 private:
  int rows;
  int cols;
  int bombCount;

  std::vector<std::vector<std::unique_ptr<Grid>>> board;

  bool gameOver;
  bool win;

 public:
 /**
   * @brief Constructor for a new Minesweeper boards.
   * @param n The number of row and column (board is n x n).
   * @param bombNo The number of bombs in the maps.
   */
  MineSweeperTable(int n, int bombNo);

  /** @brief Returns table's row. */
  int getRows() const;

  /** @brief Returns table's column. */
  int getCols() const;

  /** @brief Returns the count of all bombs in the game. */
  int getBombCount() const;

  /** @brief Returns true if the game is over (both win and lose). */
  bool isGameOver() const;

  /** @brief Returns true if the player wins the game. */
  bool isWin() const;

  /** @brief Returns board data as a 2D vector of grid. */
  const std::vector<std::vector<std::unique_ptr<Grid>>>& getBoard() const;
  
  /**
   * @brief Sets up a new game session.
   */
  void initialize();

  /**
   * @brief Reset the current game.
   */
  void reset();

  /**
   * @brief Checks if coordinates are within the board boundaries.
   * @param r Row index.
   * @param c Column index.
   */
  bool isValid(int r, int c) const;

  /**
   * @brief Place bombs randomly in the grid with the amount of bombCount.
   */
  void placeBombs();

  /**
   * @brief Compute neighbor bomb count for all grid cells.
   */
  void calculateNeighbors();

  /**
   * @brief Attempts to reveal a cell at (r, c).
   * @param r Row index.
   * @param c Column index.
   */
  void openCell(int r, int c);

  /**
   * @brief Recursively open an empty adjacent cells.
   * @param r Row index.
   * @param c Column index.
   */
  void recursiveOpen(int r, int c);

  /**
   * @brief Toggle a flag on the cell.
   */
  void toggleMark(int r, int c);

  /**
   * @brief Check if the player has reach the winning condition.
   */
  void checkWin();

 private:
 /**
   * @brief Verifies class invariants.
   * @invariant Check if the current bomb count is equal to the actual bomb count.
   * @invariant Check if the current non-bomb count is equal to the (rows * cols) - bombCount.
   * @invariant Check if the game is over when player gets the win.
   */
  void checkInvariant() const;
};

#endif
