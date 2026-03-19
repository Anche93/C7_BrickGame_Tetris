#include "tetris.h"

int main() {
  initscr();
  curs_set(0);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  timeout(0);
  srand(time(NULL));

  gameLoop();
  endwin();
  return 0;
}

void gameLoop() {
  GameState* state = ptrState();
  state->last_timestamp = getTimeMS();

  while (state->state != STATE_GAME_OVER && state->state != STATE_WIN) {
    int ch = getch();
    if (ch != ERR) {
      UserAction_t action = getAction(ch);
      bool hold = isHold(ch);
      userInput(action, hold);
    }
    updateCurrentState();
    printField(state);
    napms(40);
  }
  if (state->state == STATE_GAME_OVER || state->state == STATE_WIN) {
    if (state->info.level == WIN) {
      printGameWin();
    } else {
      printGameOver();
    }
    refresh();
    napms(5000);
  }
  getch();
  clearGameState(&state);
}
