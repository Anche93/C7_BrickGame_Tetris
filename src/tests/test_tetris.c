#include <check.h>

#include "../brick_game/tetris/tetris_backend.h"

START_TEST(test_createMatrix_1) {
  int** matrix = createMatrix(4, 4);
  // ck_assert_ptr_nonnull(matrix);
  ck_assert_ptr_ne(matrix, NULL);
  removeMatrix(matrix, 4);
}
END_TEST

START_TEST(test_createMatrix_2) {
  int** matrix1 = createMatrix(0, 0);
  // ck_assert_ptr_null(matrix1);
  ck_assert_ptr_eq(matrix1, NULL);

  int** matrix2 = createMatrix(-1, -5);
  // ck_assert_ptr_null(matrix2);
  ck_assert_ptr_eq(matrix2, NULL);

  int** matrix3 = createMatrix(3, -2);
  // ck_assert_ptr_null(matrix3);
  ck_assert_ptr_eq(matrix3, NULL);
}
END_TEST

START_TEST(test_removeMatrix_1) {
  int** matrix = createMatrix(0, 0);
  removeMatrix(matrix, 0);
  ck_assert_ptr_eq(matrix, NULL);
}
END_TEST

START_TEST(test_moveDown_1) {
  GameState* state = ptrState();
  int pos_y = state->current.y;

  moveDown(state);
  ck_assert_int_eq(state->current.y, pos_y + 1);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveDown_collide_2) {
  GameState* state = ptrState();
  nextBrick(state);
  setupNewBrick(state);
  state->current.y = 19;
  moveDown(state);
  ck_assert_int_eq(state->current.y, START_POS_Y);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveDown_collide_3) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_I;
  unpackShape(state->current.shape, mask);

  state->current.x = 5;
  state->current.y = 5;
  for (int j = 0; j < COLMS; j++) {
    state->info.field[9][j] = 1;
  }

  int res = checkCollide(state, 0, 1);
  ck_assert_int_eq(res, true);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveDown_collide_4) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_I;
  unpackShape(state->current.shape, mask);

  state->current.x = 5;
  state->current.y = 0;

  int res = checkCollide(state, 0, -1);
  ck_assert_int_eq(res, true);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveDown_collide_5) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_I;
  unpackShape(state->current.shape, mask);

  state->current.x = 0;
  state->current.y = 5;

  int res = checkCollide(state, -1, 0);
  ck_assert_int_eq(res, true);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveDown_collide_6) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_I;
  unpackShape(state->current.shape, mask);

  state->current.x = 5;
  state->current.y = 5;
  for (int i = 0; i < ROWS; i++) {
    state->info.field[i][7] = 1;
  }

  int res = checkCollide(state, 1, 0);
  ck_assert_int_eq(res, true);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveDown_collide_7) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_I;
  unpackShape(state->current.shape, mask);

  state->current.x = 5;
  state->current.y = 5;

  int res = checkCollide(state, 0, 1);
  ck_assert_int_eq(res, false);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveDown_collide_8) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_I;
  unpackShape(state->current.shape, mask);

  state->current.x = 5;
  state->current.y = 0;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLMS; j++) {
      state->info.field[i][j] = 1;
    }
  }

  int res = checkCollide(state, 0, 1);
  ck_assert_int_eq(res, true);

  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRight_1) {
  GameState* state = ptrState();
  int pos_x = state->current.x;

  moveRight(state);
  ck_assert_int_eq(state->current.x, pos_x + 1);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRight_collide_2) {
  GameState* state = ptrState();
  nextBrick(state);
  setupNewBrick(state);
  state->current.x = COLMS;

  moveRight(state);
  ck_assert_int_eq(state->current.x, COLMS);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveLeft_1) {
  GameState* state = ptrState();
  int pos_x = state->current.x;

  moveLeft(state);
  ck_assert_int_eq(state->current.x, pos_x - 1);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveLeft_collide_2) {
  GameState* state = ptrState();
  nextBrick(state);
  setupNewBrick(state);
  state->current.x = 0;

  moveLeft(state);
  ck_assert_int_eq(state->current.x, 0);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRotation_1) {
  GameState* state = ptrState();
  nextBrick(state);
  setupNewBrick(state);
  int pos_y = state->current.y;

  moveRotation(state);
  ck_assert_int_eq(state->current.y, pos_y);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRotation_Right_3) {
  GameState* state = ptrState();
  nextBrick(state);
  setupNewBrick(state);
  state->current.x = COLMS;

  moveRotation(state);
  ck_assert_int_eq(state->current.x, COLMS);
  ck_assert_uint_ge(state->last_timestamp, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRotation_collide_4) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_O;
  int temp[BRICK_ROW][BRICK_COLM] = {0};
  unpackShape(temp, mask);

  state->current.x = 5;
  state->current.y = ROWS - 1;
  int size = 2;
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      state->current.shape[i][j] = temp[i][j];
    }
  }

  bool res = checkRotation(state, temp, size);
  ck_assert_int_eq(res, true);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRotation_collide_5) {
  GameState* state = ptrState();
  unsigned short mask = SHAPE_O;
  int temp[BRICK_ROW][BRICK_COLM] = {0};
  unpackShape(temp, mask);

  state->current.x = 5;
  state->current.y = 5;
  int size = 2;
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      state->current.shape[i][j] = temp[i][j];
    }
  }

  state->info.field[5][5] = 1;
  state->info.field[5][6] = 1;
  state->info.field[6][5] = 1;
  state->info.field[6][6] = 1;

  rotation(state);

  bool res = checkRotation(state, temp, size);
  ck_assert_int_eq(res, true);
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRotation_collide_6) {
  GameState* state = ptrState();
  unsigned short mask = 0xCC03;
  unpackShape(state->current.shape, mask);

  state->current.x = 5;
  state->current.y = 5;

  state->info.field[5][5] = 1;
  state->info.field[5][6] = 1;
  state->info.field[6][5] = 1;
  state->info.field[6][6] = 1;

  rotation(state);
  for (int i = 3; i < BRICK_ROW; i++) {
    for (int j = 2; j < BRICK_COLM; j++) {
      ck_assert_int_eq(state->current.shape[i][j], 0);
    }
  }
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRotation_collide_7) {
  GameState* state = ptrState();
  unsigned short mask_1 = SHAPE_J;
  unpackShape(state->current.shape, mask_1);

  state->current.x = 5;
  state->current.y = 5;

  rotation(state);
  unsigned short mask_2 = 0x8E00;
  int temp_2[BRICK_ROW][BRICK_COLM] = {0};
  unpackShape(temp_2, mask_2);

  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      ck_assert_int_eq(state->current.shape[i][j], temp_2[i][j]);
    }
  }
  clearGameState(&state);
}
END_TEST

START_TEST(test_moveRotation_collide_8) {
  GameState* state = ptrState();
  unsigned short mask_1 = SHAPE_J;
  int temp_1[BRICK_ROW][BRICK_COLM] = {0};
  unpackShape(temp_1, mask_1);

  state->current.x = 5;
  state->current.y = 5;
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      state->current.shape[i][j] = temp_1[i][j];
    }
  }
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLMS; j++) {
      state->info.field[i][j] = 1;
    }
  }
  rotation(state);

  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      ck_assert_int_eq(state->current.shape[i][j], temp_1[i][j]);
    }
  }
  clearGameState(&state);
}
END_TEST

START_TEST(test_shiftRotation_1) {
  GameState* state = ptrState();
  state->current.x = -1;
  state->current.y = 5;
  int size = 3;

  shiftRotation(state, size);
  ck_assert_int_eq(state->current.x, 0);
  ck_assert_int_eq(state->current.y, 5);
  clearGameState(&state);
}
END_TEST

START_TEST(test_shiftRotation_2) {
  GameState* state = ptrState();
  state->current.x = COLMS - 2;
  state->current.y = 5;
  int size = 3;

  shiftRotation(state, size);
  ck_assert_int_eq(state->current.x, COLMS - 3);
  ck_assert_int_eq(state->current.y, 5);
  clearGameState(&state);
}
END_TEST

START_TEST(test_shiftRotation_3) {
  GameState* state = ptrState();
  state->current.x = 5;
  state->current.y = -1;
  int size = 3;

  shiftRotation(state, size);
  ck_assert_int_eq(state->current.x, 5);
  ck_assert_int_eq(state->current.y, 0);
  clearGameState(&state);
}
END_TEST

START_TEST(test_shiftRotation_4) {
  GameState* state = ptrState();
  state->current.x = 5;
  state->current.y = ROWS - 2;
  int size = 3;

  shiftRotation(state, size);
  ck_assert_int_eq(state->current.x, 5);
  ck_assert_int_eq(state->current.y, ROWS - 3);

  levelUp(state);
  ck_assert_int_eq(state->info.score, 0);

  clearGameState(&state);
}
END_TEST

START_TEST(test_fullLine_1) {
  GameState* state = ptrState();
  for (int i = ROWS - 1; i < ROWS; i++) {
    for (int j = 0; j < COLMS - 1; j++) {
      state->info.field[i][j] = 1;
    }
  }
  fullLine(state);
  ck_assert_int_eq(state->info.score, 100);
  clearGameState(&state);
}
END_TEST

START_TEST(test_fullLine_2) {
  GameState* state = ptrState();
  for (int i = ROWS - 2; i < ROWS; i++) {
    for (int j = 0; j < COLMS - 1; j++) {
      state->info.field[i][j] = 1;
    }
  }
  fullLine(state);
  ck_assert_int_eq(state->info.score, 300);
  clearGameState(&state);
}
END_TEST

START_TEST(test_fullLine_3) {
  GameState* state = ptrState();
  for (int i = ROWS - 3; i < ROWS; i++) {
    for (int j = 0; j < COLMS - 1; j++) {
      state->info.field[i][j] = 1;
    }
  }
  fullLine(state);
  ck_assert_int_eq(state->info.score, 700);
  clearGameState(&state);
}
END_TEST

START_TEST(test_fullLine_4) {
  GameState* state = ptrState();
  for (int i = ROWS - 4; i < ROWS; i++) {
    for (int j = 0; j < COLMS - 1; j++) {
      state->info.field[i][j] = 1;
    }
  }
  fullLine(state);
  ck_assert_int_eq(state->info.score, 1500);

  levelUp(state);
  ck_assert_int_eq(state->info.level, 3);
  ck_assert_int_eq(state->info.speed, 800);

  clearGameState(&state);
}
END_TEST

START_TEST(test_fsm_1) {
  GameState* state = ptrState();
  state->state = STATE_PLAY;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLMS; j++) {
      state->info.field[i][j] = 1;
    }
  }
  unsigned short mask = SHAPE_I;
  int temp[BRICK_ROW][BRICK_COLM] = {0};
  unpackShape(temp, mask);
  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      state->info.next[i][j] = temp[i][j];
    }
  }
  spawnNewBrick(state);
  ck_assert_int_eq(state->state, STATE_GAME_OVER);
  userInput(Left, false);
  ck_assert_int_eq(state->state, STATE_GAME_OVER);
  clearGameState(&state);
}
END_TEST

START_TEST(test_fsm_2) {
  GameState* state = ptrState();
  state->state = STATE_START;

  userInput(Start, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  userInput(Pause, false);
  ck_assert_int_eq(state->state, STATE_PAUSE);
  manageGameState(state, Terminate);
  ck_assert_int_eq(state->state, STATE_GAME_OVER);

  clearGameState(&state);
}
END_TEST

START_TEST(test_fsm_3) {
  GameState* state = ptrState();
  state->state = STATE_PLAY;

  manageGameState(state, Pause);
  ck_assert_int_eq(state->state, STATE_PAUSE);
  manageGameState(state, Start);
  ck_assert_int_eq(state->state, STATE_PAUSE);
  updateCurrentState();
  manageGameState(state, Pause);
  ck_assert_int_eq(state->state, STATE_PLAY);

  userInput(Start, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  userInput(Terminate, false);
  ck_assert_int_eq(state->state, STATE_GAME_OVER);

  clearGameState(&state);
}
END_TEST

START_TEST(test_fsm_4) {
  GameState* state = ptrState();
  state->state = STATE_START;

  userInput(Left, false);
  ck_assert_int_eq(state->state, STATE_START);
  userInput(Start, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  userInput(Left, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  userInput(Right, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  userInput(Up, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  userInput(Down, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  userInput(Down, true);
  ck_assert_int_eq(state->state, STATE_PLAY);
  int res = checkCollide(state, 0, 1);
  ck_assert_int_eq(res, true);
  userInput(Action, false);
  ck_assert_int_eq(state->state, STATE_PLAY);
  updateCurrentState();
  ck_assert_uint_ge(state->last_timestamp, 0);

  state->info.level = 11;
  updateCurrentState();
  ck_assert_int_eq(state->state, STATE_WIN);
  userInput(Left, false);
  ck_assert_int_eq(state->state, STATE_WIN);

  clearGameState(&state);
}
END_TEST

START_TEST(test_fsm_5) {
  GameState* state = ptrState();
  state->state = STATE_PLAY;
  state->info.pause = true;
  goToStart(state);
  ck_assert_int_eq(state->state, STATE_START);
  ck_assert_int_eq(state->info.pause, false);
  clearGameState(&state);
}
END_TEST

START_TEST(test_updateCurrentState_1) {
  GameState* state = ptrState();

  state->state = STATE_PLAY;
  state->info.pause = false;
  state->info.speed = 100;
  state->last_timestamp = getTimeMS() - 200;
  int pos_y = state->current.y;

  updateCurrentState();
  ck_assert_int_eq(state->current.y, pos_y + 1);
  ck_assert_uint_ge(state->last_timestamp, getTimeMS() - 10);
  clearGameState(&state);
}
END_TEST

START_TEST(test_initClearGame_1) {
  GameState* state = NULL;
  initGame(&state);

  // ck_assert_ptr_nonnull(state);
  ck_assert_ptr_ne(state, NULL);
  ck_assert_int_eq(state->info.score, 0);
  ck_assert_int_eq(state->info.level, DEFAULT_LVL);
  ck_assert_int_eq(state->info.speed, DEFAULT_SPEED);
  ck_assert_int_eq(state->info.pause, false);

  // ck_assert_ptr_nonnull(state->info.field);
  ck_assert_ptr_ne(state->info.field, NULL);
  // ck_assert_ptr_nonnull(state->info.next);
  ck_assert_ptr_ne(state->info.next, NULL);

  ck_assert_int_eq(state->current.x, START_POS_X);
  ck_assert_int_eq(state->current.y, START_POS_Y);
  ck_assert_int_eq(state->state, STATE_START);

  for (int i = 0; i < BRICK_ROW; i++) {
    for (int j = 0; j < BRICK_COLM; j++) {
      ck_assert_int_eq(state->current.shape[i][j], 0);
    }
  }
  clearGameState(&state);
}
END_TEST

START_TEST(test_initClearGame_2) {
  GameState* state = NULL;
  clearGameState(&state);
}
END_TEST

START_TEST(test_highScore_1) {
  GameState* state = NULL;
  initGame(&state);

  state->info.score = 2500;
  updateHighscoreDB(state);
  saveHighscoreDB(state->info.score, state->info.level);
  ck_assert_int_eq(state->info.high_score, 2500);
  clearGameState(&state);
}
END_TEST

START_TEST(test_highScore_2) {
  GameState* state = ptrState();

  state->info.high_score = 1000;
  state->info.score = 1500;
  state->info.level = 5;

  updateHighscoreDB(state);

  ck_assert_int_eq(state->info.high_score, 1500);
  clearGameState(&state);
}
END_TEST

Suite* tetris_suite(void) {
  Suite* s = suite_create("Tetris Tests");

  TCase* tc_create = tcase_create("Create matrix");
  tcase_add_test(tc_create, test_createMatrix_1);
  tcase_add_test(tc_create, test_createMatrix_2);
  suite_add_tcase(s, tc_create);

  TCase* tc_remove = tcase_create("Remove matrix");
  tcase_add_test(tc_remove, test_removeMatrix_1);
  suite_add_tcase(s, tc_remove);

  TCase* tc_moveDown = tcase_create("Move down");
  tcase_add_test(tc_moveDown, test_moveDown_1);
  tcase_add_test(tc_moveDown, test_moveDown_collide_2);
  tcase_add_test(tc_moveDown, test_moveDown_collide_3);
  tcase_add_test(tc_moveDown, test_moveDown_collide_4);
  tcase_add_test(tc_moveDown, test_moveDown_collide_5);
  tcase_add_test(tc_moveDown, test_moveDown_collide_6);
  tcase_add_test(tc_moveDown, test_moveDown_collide_7);
  tcase_add_test(tc_moveDown, test_moveDown_collide_8);
  suite_add_tcase(s, tc_moveDown);

  TCase* tc_moveRight = tcase_create("Move Right");
  tcase_add_test(tc_moveRight, test_moveRight_1);
  tcase_add_test(tc_moveRight, test_moveRight_collide_2);
  suite_add_tcase(s, tc_moveRight);

  TCase* tc_moveLeft = tcase_create("Move Left");
  tcase_add_test(tc_moveLeft, test_moveLeft_1);
  tcase_add_test(tc_moveLeft, test_moveLeft_collide_2);
  suite_add_tcase(s, tc_moveLeft);

  TCase* tc_moveRotation = tcase_create("Move Rotation");
  tcase_add_test(tc_moveRotation, test_moveRotation_1);
  tcase_add_test(tc_moveRotation, test_moveRotation_Right_3);
  tcase_add_test(tc_moveRotation, test_moveRotation_collide_4);
  tcase_add_test(tc_moveRotation, test_moveRotation_collide_5);
  tcase_add_test(tc_moveRotation, test_moveRotation_collide_6);
  tcase_add_test(tc_moveRotation, test_moveRotation_collide_7);
  tcase_add_test(tc_moveRotation, test_moveRotation_collide_8);
  suite_add_tcase(s, tc_moveRotation);

  TCase* tc_shiftRotation = tcase_create("Shift Rotation");
  tcase_add_test(tc_shiftRotation, test_shiftRotation_1);
  tcase_add_test(tc_shiftRotation, test_shiftRotation_2);
  tcase_add_test(tc_shiftRotation, test_shiftRotation_3);
  tcase_add_test(tc_shiftRotation, test_shiftRotation_4);
  suite_add_tcase(s, tc_shiftRotation);

  TCase* tc_fullLine = tcase_create("Full Line");
  tcase_add_test(tc_fullLine, test_fullLine_1);
  tcase_add_test(tc_fullLine, test_fullLine_2);
  tcase_add_test(tc_fullLine, test_fullLine_3);
  tcase_add_test(tc_fullLine, test_fullLine_4);
  suite_add_tcase(s, tc_fullLine);

  TCase* tc_fsm = tcase_create("Test FSM");
  tcase_add_test(tc_fsm, test_fsm_1);
  tcase_add_test(tc_fsm, test_fsm_2);
  tcase_add_test(tc_fsm, test_fsm_3);
  tcase_add_test(tc_fsm, test_fsm_4);
  tcase_add_test(tc_fsm, test_fsm_5);
  suite_add_tcase(s, tc_fsm);

  TCase* tc_updateCurrentState = tcase_create("Update Current State");
  tcase_add_test(tc_updateCurrentState, test_updateCurrentState_1);
  suite_add_tcase(s, tc_updateCurrentState);

  TCase* tc_initClearGame = tcase_create("Initialization the game");
  tcase_add_test(tc_initClearGame, test_initClearGame_1);
  tcase_add_test(tc_initClearGame, test_initClearGame_2);
  suite_add_tcase(s, tc_initClearGame);

  TCase* tc_highScore = tcase_create("High Score");
  tcase_add_test(tc_highScore, test_highScore_1);
  tcase_add_test(tc_highScore, test_highScore_2);
  suite_add_tcase(s, tc_highScore);

  return s;
}

int main(void) {
  int number_failed = 0;
  Suite* s = tetris_suite();
  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}