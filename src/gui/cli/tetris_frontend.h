#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H

#include <ncurses.h>
#include <time.h>

#include "../../brick_game/tetris/tetris_backend.h"

#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)

#define ROWS_MAP 20
#define COLMS_MAP 10

#define BOARDS_BEGIN 2

#define BOARD_W (COLMS_MAP + MAP_PADDING * 2)
#define BOARD_H (ROWS_MAP + MAP_PADDING * 2)
#define HUD_WIDTH 12
#define MAP_PADDING 3

// Tetris_frontend.c
void printOverlay(void);
void printRectangle(int top_y, int bottom, int left_x, int right_x);
void printState(GameState* state);
void printField(GameState* state);
void printShape(GameState* state);
void printNextShape(const GameState* state);
void printGameStart(void);
void printPause(void);
void printGameOver(void);
void printGameWin(void);

// User_imput.c
UserAction_t getAction(int ch);
bool isHold(int ch);

#endif