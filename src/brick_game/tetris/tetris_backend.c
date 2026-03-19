#include "tetris_backend.h"

void unpackShape(int shape[BRICK_ROW][BRICK_COLM], unsigned short mask) {
  for (int y = 0; y < BRICK_ROW; y++) {
    for (int x = 0; x < BRICK_COLM; x++) {
      shape[y][x] = (mask >> (15 - (y * 4 + x))) & 1;
    }
  }
}

void nextBrick(GameState* state) {
  int shape[BRICK_ROW][BRICK_COLM] = {0};
  const unsigned short masks[MASKS] = {SHAPE_I, SHAPE_J, SHAPE_L, SHAPE_O,
                                       SHAPE_S, SHAPE_T, SHAPE_Z};
  unsigned short mask = masks[rand() % MASKS];
  unpackShape(shape, mask);
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      state->info.next[i][j] = shape[i][j];
    }
  }
}

void setupNewBrick(GameState* state) {
  state->current.x = START_POS_X;
  state->current.y = START_POS_Y;

  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      state->current.shape[i][j] = state->info.next[i][j];
    }
  }
  nextBrick(state);
}

void holdBrick(GameState* state) {
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      if (state->current.shape[i][j] != 0) {
        int field_x = state->current.x + j;
        int field_y = state->current.y + i;
        if (field_y >= 0 && field_y < ROWS && field_x >= 0 && field_x < COLMS)
          state->info.field[field_y][field_x] = state->current.shape[i][j];
      }
    }
  }
  fullLine(state);
}

void fullLine(GameState* state) {
  int count_line = 0;
  for (int i = 0; i < ROWS; i++) {
    int count_elem = 0;
    for (int j = 0; j < COLMS; j++) {
      if (state->info.field[i][j] != 0) count_elem++;
    }
    if (count_elem == COLMS - 1) {
      count_line++;
      cleanDownLine(state, i);
      i--;
    }
  }
  if (count_line == 1) state->info.score += 100;
  if (count_line == 2) state->info.score += 300;
  if (count_line == 3) state->info.score += 700;
  if (count_line == 4) state->info.score += 1500;

  updateHighscoreDB(state);
}

void cleanDownLine(GameState* state, int x) {
  for (int i = x; i > 0; i--) {
    for (int j = 0; j < COLMS; j++) {
      state->info.field[i][j] = state->info.field[i - 1][j];
    }
  }
  for (int j = 0; j < COLMS; j++) {
    state->info.field[0][j] = 0;
  }
}

bool checkCollide(GameState* state, int x, int y) {
  bool flg = false;
  for (int i = 0; i < BRICK_ROW && !flg; i++) {
    for (int j = 0; j < BRICK_COLM && !flg; j++) {
      if (state->current.shape[i][j] != 0) {
        int new_x = state->current.x + j + x;
        int new_y = state->current.y + i + y;

        if (new_x <= 0 || new_x >= COLMS)
          flg = true;
        else if (new_y < 0 || new_y >= ROWS)
          flg = true;
        else if (state->info.field[new_y][new_x] != 0)
          flg = true;
      }
    }
  }
  return flg;
}

bool checkRotation(GameState* state, const int temp[BRICK_ROW][BRICK_COLM],
                   int size) {
  bool flg = false;
  for (int i = 0; i < size && !flg; i++) {
    for (int j = 0; j < size && !flg; j++) {
      if (temp[i][j] != 0) {
        int x = state->current.x + j;
        int y = state->current.y + i;
        if (y >= ROWS)
          flg = true;
        else if (state->info.field[y][x] != 0)
          flg = true;
      }
    }
  }
  return flg;
}

void rotation(GameState* state) {
  int temp[BRICK_ROW][BRICK_COLM] = {0};
  int size = 0;
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      if (state->current.shape[i][j] != 0) {
        if (i + 1 > size) size = 1 + i;
        if (j + 1 > size) size = 1 + j;
      }
    }
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      temp[i][j] = state->current.shape[size - 1 - j][i];
    }
  }
  if (!checkRotation(state, temp, size)) {
    for (int i = 0; i < BRICK_ROW; i++) {
      for (int j = 0; j < BRICK_COLM; j++) {
        state->current.shape[i][j] = temp[i][j];
      }
    }
    for (int i = size; i < BRICK_ROW; i++) {
      for (int j = size; j < BRICK_COLM; j++) {
        state->current.shape[i][j] = 0;
      }
    }
    shiftRotation(state, size);
  }
}

void shiftRotation(GameState* state, int size) {
  int min_x = state->current.x;
  int max_x = state->current.x + size - 1;

  if (min_x <= 0) {
    state->current.x += 1;
  } else if (max_x >= COLMS) {
    state->current.x -= (max_x - (COLMS - 1));
  }

  int min_y = state->current.y;
  int max_y = state->current.y + size - 1;

  if (min_y < 0) {
    state->current.y += -min_y;
  } else if (max_y >= ROWS) {
    state->current.y -= (max_y - (ROWS - 1));
  }
}

unsigned int getTimeMS(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (unsigned int)ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}

void levelUp(GameState* state) {
  if (state->info.score > 0) {
    state->info.level = (state->info.score / FOR_UP_LVL) + 1;
    state->info.speed = DEFAULT_SPEED - ((state->info.level - 1) * 100);
  }
}