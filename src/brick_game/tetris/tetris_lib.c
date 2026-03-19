#include "tetris_lib.h"

#include "tetris_backend.h"

GameState* ptrState() {
  static GameState* state = NULL;
  if (!state) {
    initGame(&state);
  }
  return state;
}

void userInput(UserAction_t action, bool hold) {
  GameState* state = ptrState();

  manageGameState(state, action);

  if (state->state == STATE_PLAY) {
    switch (action) {
      case Left:
        moveLeft(state);
        break;
      case Right:
        moveRight(state);
        break;
      case Up:
        break;
      case Down:
        moveDown(state);
        if (hold) {
          while (!checkCollide(state, 0, 1)) {
            moveDown(state);
          }
        }
        break;
      case Action:
        moveRotation(state);
        break;
      default:
        break;
    }
  }
}

GameInfo_t updateCurrentState() {
  GameState* state = ptrState();

  if (state->state == STATE_PLAY && !state->info.pause) {
    unsigned int now = getTimeMS();
    double pass_time = 0.0;
    pass_time = (double)(now - state->last_timestamp);

    if (pass_time >= state->info.speed) {
      moveDown(state);
      state->last_timestamp = now;
    }
    levelUp(state);

    if (state->info.level == WIN) {
      goToWin(state);
    }
  }
  return state->info;
}