#include "tetris_frontend.h"

void printOverlay(void) {
  printRectangle(0, BOARD_H + 1, 0, BOARD_W + 1);
  printRectangle(MAP_PADDING, ROWS_MAP + MAP_PADDING + 1, MAP_PADDING,
                 COLMS_MAP + MAP_PADDING + 1);
  printRectangle(0, BOARD_H + 1, BOARD_W + 2, BOARD_W + HUD_WIDTH + 3);

  printRectangle(3, 8, BOARD_W + 3, BOARD_W + HUD_WIDTH + 2);
  printRectangle(10, 12, BOARD_W + 3, BOARD_W + HUD_WIDTH + 2);
  printRectangle(14, 16, BOARD_W + 3, BOARD_W + HUD_WIDTH + 2);
  printRectangle(18, 20, BOARD_W + 3, BOARD_W + HUD_WIDTH + 2);
  printRectangle(22, 24, BOARD_W + 3, BOARD_W + HUD_WIDTH + 2);

  MVPRINTW(2, BOARD_W + 5, "NEXT:");
  MVPRINTW(9, BOARD_W + 5, "SCORE:");
  MVPRINTW(13, BOARD_W + 5, "HI-SCORE:");
  MVPRINTW(17, BOARD_W + 5, "LEVEL:");
  MVPRINTW(21, BOARD_W + 5, "SPEED:");
}

void printRectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;
  for (; i < right_x; i++) {
    MVADDCH(top_y, i, ACS_HLINE);
  }
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int j = top_y + 1; j < bottom_y; j++) {
    MVADDCH(j, left_x, ACS_VLINE);
    MVADDCH(j, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) {
    MVADDCH(bottom_y, i, ACS_HLINE);
  }
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void printState(GameState* state) {
  MVPRINTW(11, BOARD_W + 5, "%d", state->info.score);
  MVPRINTW(15, BOARD_W + 5, "%d", state->info.high_score);
  MVPRINTW(19, BOARD_W + 5, "%d", state->info.level);
  MVPRINTW(23, BOARD_W + 5, "%d", state->info.speed);
}

void printField(GameState* state) {
  clear();
  printOverlay();
  printState(state);

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLMS; j++) {
      if (state->info.field[i][j] != 0)
        MVADDCH(i + MAP_PADDING, j + MAP_PADDING, ACS_CKBOARD);
    }
  }
  printShape(state);
  printNextShape(state);

  if (state->state == STATE_START) {
    printGameStart();
  }
  if (state->state == STATE_PAUSE) {
    printPause();
  }
  refresh();
}

void printShape(GameState* state) {
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      if (state->current.shape[i][j] != 0) {
        int pos_y = state->current.y + i + MAP_PADDING;
        int pos_x = state->current.x + j + MAP_PADDING;
        MVADDCH(pos_y, pos_x, ACS_CKBOARD);
      }
    }
  }
}

void printNextShape(const GameState* state) {
  int y = 4;
  int x = BOARD_W + 3 + (HUD_WIDTH - 4) / 2;

  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      MVADDCH(y + i, x + j, ' ');
    }
  }
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      if (state->info.next[i][j] != 0) {
        MVADDCH(y + i, x + j, ACS_CKBOARD);
      }
    }
  }
}

void printGameStart(void) {
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 - 1, COLMS_MAP / 2 - 3,
           "==============");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2, COLMS_MAP / 2 - 2, " Press ENTER ");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 + 1, COLMS_MAP / 2 - 3,
           "==============");
}

void printPause(void) {
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 - 1, COLMS_MAP / 2 - 3,
           "==============");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2, COLMS_MAP / 2 - 2, "    PAUSE    ");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 + 1, COLMS_MAP / 2 - 3,
           "==============");
}

void printGameOver(void) {
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 - 1, COLMS_MAP / 2 - 3,
           "==============");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2, COLMS_MAP / 2 - 2, "  GAME OVER  ");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 + 1, COLMS_MAP / 2 - 3,
           "==============");
}

void printGameWin(void) {
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 - 1, COLMS_MAP / 2 - 3,
           "==============");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2, COLMS_MAP / 2 - 2, "!! YOU WON !!");
  MVPRINTW((ROWS_MAP + MAP_PADDING) / 2 + 1, COLMS_MAP / 2 - 3,
           "==============");
}