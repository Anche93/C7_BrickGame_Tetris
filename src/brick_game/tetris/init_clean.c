#include "tetris_backend.h"

void initGame(GameState** state) {
  *state = (GameState*)malloc(sizeof(GameState));
  if (*state != NULL) {
    bool flg = true;
    (*state)->info.field = createMatrix(ROWS, COLMS);
    (*state)->info.next = createMatrix(BRICK_ROW, BRICK_COLM);

    if ((*state)->info.field == NULL || (*state)->info.next == NULL) {
      if ((*state)->info.field) removeMatrix((*state)->info.field, ROWS);
      if ((*state)->info.next) removeMatrix((*state)->info.next, BRICK_ROW);
      free(*state);
      *state = NULL;
      flg = false;
    }
    if (flg) {
      (*state)->info.score = 0;
      if (initHighscoreDB() == 0) {
        (*state)->info.high_score = loadHighscoreDB();
      }
      (*state)->info.level = DEFAULT_LVL;
      (*state)->info.speed = DEFAULT_SPEED;
      (*state)->info.pause = false;

      (*state)->current.x = START_POS_X;
      (*state)->current.y = START_POS_Y;
      memset((*state)->current.shape, 0, sizeof((*state)->current.shape));

      (*state)->last_timestamp = getTimeMS();
      (*state)->state = STATE_START;
    }
  }
}

void clearGameState(GameState** state) {
  if (*state != NULL) {
    if ((*state)->info.field != NULL) {
      removeMatrix((*state)->info.field, ROWS);
      (*state)->info.field = NULL;
    }
    if ((*state)->info.next != NULL) {
      removeMatrix((*state)->info.next, BRICK_ROW);
      (*state)->info.next = NULL;
    }
    memset(&(*state)->info, 0, sizeof(GameInfo_t));

    if ((*state)->info.score > 0) {
      saveHighscoreDB((*state)->info.score, (*state)->info.level);
    }

    free(*state);
    *state = NULL;
  }
}