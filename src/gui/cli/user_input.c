#include "tetris_frontend.h"

static struct {
  int key;
  clock_t timestamp;
} key_hold = {0, 0};

UserAction_t getAction(int ch) {
  switch (ch) {
    case '\n':
      return Start;
    case 'p':
    case 'P':
      return Pause;
    case 'q':
    case 'Q':
      return Terminate;
    case KEY_LEFT:
      return Left;
    case KEY_RIGHT:
      return Right;
    case KEY_UP:
      return Up;
    case KEY_DOWN:
      return Down;
    case ' ':
      return Action;
    default:
      return Up;
  }
}

bool isHold(int ch) {
  bool flg = false;
  clock_t now = clock();

  if (key_hold.key != ch) {
    key_hold.key = ch;
    key_hold.timestamp = now;
    flg = false;
  }
  if (((now - key_hold.timestamp) * 1000 / CLOCKS_PER_SEC) > 300) {
    flg = true;
  }
  return flg;
}