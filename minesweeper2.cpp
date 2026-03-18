#include <SDL.h>
#include <SDL_ttf.h>

#include <ctime>
#include <iostream>
#include <string>

#include "grid.h"
#include "minesweepertable.h"

using namespace std;

/* =====================
   CONFIG
===================== */

const int CELL_SIZE = 32;
const int UI_HEIGHT = 60;
const int FONT_SIZE = 18;

/* =====================
   Text Render
===================== */

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font,
                        const string& text, SDL_Color color) {
  SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), color);

  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);

  SDL_FreeSurface(surf);

  return tex;
}

/* =====================
   Draw
===================== */

void drawBoard(SDL_Renderer* renderer, TTF_Font* font,
               const MineSweeperTable& game, SDL_Rect resetBtn) {
  const auto& board = game.getBoard();

  /* UI Background */

  SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
  SDL_Rect ui{0, 0, game.getCols() * CELL_SIZE, UI_HEIGHT};
  SDL_RenderFillRect(renderer, &ui);

  /* Reset Button */

  SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
  SDL_RenderFillRect(renderer, &resetBtn);

  SDL_Texture* resetText = renderText(renderer, font, "RESET", {255, 255, 255});

  int tw, th;
  SDL_QueryTexture(resetText, nullptr, nullptr, &tw, &th);

  SDL_Rect tr{resetBtn.x + (resetBtn.w - tw) / 2,
              resetBtn.y + (resetBtn.h - th) / 2, tw, th};

  SDL_RenderCopy(renderer, resetText, nullptr, &tr);

  SDL_DestroyTexture(resetText);

  /* Bomb Counter */

  string bombText = "Bombs: " + to_string(game.getBombCount());

  SDL_Texture* bombTex = renderText(renderer, font, bombText, {0, 0, 0});

  SDL_QueryTexture(bombTex, nullptr, nullptr, &tw, &th);

  SDL_Rect br{200, 15, tw, th};

  SDL_RenderCopy(renderer, bombTex, nullptr, &br);

  SDL_DestroyTexture(bombTex);

  /* Grid */

  for (int r = 0; r < game.getRows(); r++) {
    for (int c = 0; c < game.getCols(); c++) {
      SDL_Rect rect{c * CELL_SIZE, r * CELL_SIZE + UI_HEIGHT, CELL_SIZE,
                    CELL_SIZE};

      const Grid& cell = *board[r][c];

      /* Background */

      if (cell.getIsOpen())
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
      else
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);

      SDL_RenderFillRect(renderer, &rect);

      /* Border */

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderDrawRect(renderer, &rect);

      /* Flag */

      if (cell.getMarked()) {
        if (game.isWin())
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else
          SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        SDL_Rect flag{rect.x + 8, rect.y + 8, 16, 16};

        SDL_RenderFillRect(renderer, &flag);
      }

      /* Bomb */

      if (game.isGameOver() && cell.getHasBomb()) {
        if (game.isWin())
          SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_Rect bomb{rect.x + 6, rect.y + 6, 20, 20};

        SDL_RenderFillRect(renderer, &bomb);
      }

      /* Number */

      if (cell.getIsOpen() && !cell.getHasBomb() &&
          cell.getNeighborBombCount() > 0) {
        string num = to_string(cell.getNeighborBombCount());

        SDL_Texture* numTex = renderText(renderer, font, num, {0, 0, 255});

        SDL_QueryTexture(numTex, nullptr, nullptr, &tw, &th);

        SDL_Rect nr{rect.x + (CELL_SIZE - tw) / 2,
                    rect.y + (CELL_SIZE - th) / 2, tw, th};

        SDL_RenderCopy(renderer, numTex, nullptr, &nr);

        SDL_DestroyTexture(numTex);
      }
    }
  }
}

/* =====================
   MAIN
===================== */

int main(int argc, char* argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
  srand((unsigned)time(nullptr));
  MineSweeperTable game(10, 10);

  int width = game.getCols() * CELL_SIZE;
  int height = game.getRows() * CELL_SIZE + UI_HEIGHT;

  SDL_Window* window =
      SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width, height, 0);

  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  TTF_Font* font = TTF_OpenFont("Chango-regular.ttf", FONT_SIZE);

  if (!font) {
    cout << "Font not found!\n";
    return 1;
  }

  game.initialize();

  SDL_Rect resetBtn{20, 10, 120, 40};

  bool running = true;

  while (running) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;

      if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx = e.button.x;
        int my = e.button.y;

        SDL_Point mousePoint{mx, my};

        /* Reset button */
        if (SDL_PointInRect(&mousePoint, &resetBtn)) {
          game.reset();
          continue;
        }

        if (!game.isGameOver()) {
          int c = mx / CELL_SIZE;
          int r = (my - UI_HEIGHT) / CELL_SIZE;

          if (r >= 0 && r < game.getRows() && c >= 0 && c < game.getCols()) {
            if (e.button.button == SDL_BUTTON_LEFT) {
              game.openCell(r, c);
              game.checkWin();
            }

            if (e.button.button == SDL_BUTTON_RIGHT) {
              game.toggleMark(r, c);
              game.checkWin();
            }
          }
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);

    drawBoard(renderer, font, game, resetBtn);

    SDL_RenderPresent(renderer);

    SDL_Delay(16);
  }

  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();

  return 0;
}
