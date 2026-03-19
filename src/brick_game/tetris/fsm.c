#include "tetris_backend.h"

void moveDown(GameState* state) {
  if (!checkCollide(state, 0, 1)) {
    state->current.y++;
    state->last_timestamp = getTimeMS();
  } else {
    holdBrick(state);
    spawnNewBrick(state);
    state->last_timestamp = getTimeMS();
  }
}

void moveRight(GameState* state) {
  if (!checkCollide(state, 1, 0)) {
    state->current.x++;
  }
}

void moveLeft(GameState* state) {
  if (!checkCollide(state, -1, 0)) {
    state->current.x--;
  }
}

void moveRotation(GameState* state) {
  if (!checkCollide(state, 0, 0)) {
    rotation(state);
  }
}

void spawnNewBrick(GameState* state) {
  setupNewBrick(state);

  if (checkCollide(state, 0, 0)) {
    goToGameOver(state);
  }
}

void goToStart(GameState* state) {
  state->state = STATE_START;
  state->info.pause = false;
}

void goToPlay(GameState* state) {
  if (state->state == STATE_START || state->state == STATE_PAUSE) {
    state->state = STATE_PLAY;
    state->info.pause = false;
  }
}

void goToPause(GameState* state) {
  if (state->state == STATE_PLAY) {
    state->state = STATE_PAUSE;
    state->info.pause = true;
  }
}

void goToGameOver(GameState* state) { state->state = STATE_GAME_OVER; }

void goToWin(GameState* state) { state->state = STATE_WIN; }

void manageGameState(GameState* state, UserAction_t action) {
  switch (state->state) {
    case STATE_START:
      if (action == Start) {
        goToPlay(state);
        nextBrick(state);
        setupNewBrick(state);
      }
      break;
    case STATE_PLAY:
      switch (action) {
        case Pause:
          goToPause(state);
          break;
        case Terminate:
          goToGameOver(state);
          break;
        default:
          break;
      }
      break;
    case STATE_PAUSE:
      if (action == Pause) {
        goToPlay(state);
      } else if (action == Terminate) {
        goToGameOver(state);
      }
      break;
    case STATE_GAME_OVER:
    case STATE_WIN:
      break;
  }
}