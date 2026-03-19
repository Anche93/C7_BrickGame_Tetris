#include "tetris_backend.h"

int initHighscoreDB(void) {
  int error = 0;
  sqlite3* db = NULL;
  char* err_msg = NULL;

  error = sqlite3_open(DB_FILE, &db);
  if (error != SQLITE_OK) {
    error = -1;
  } else {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS highscore ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "score INTEGER NOT NULL, "
        "level INTEGER NOT NULL, "
        "date TIMESTAMP DEFAULT CURRENT_TIMESTAMP);";
    error = sqlite3_exec(db, sql, NULL, 0, &err_msg);
    if (error != SQLITE_OK) {
      sqlite3_free(err_msg);
      error = -1;
    }
  }
  if (db != NULL) {
    sqlite3_close(db);
  }
  return error;
}

int loadHighscoreDB(void) {
  int error = 0;
  sqlite3* db = NULL;
  sqlite3_stmt* stmt = NULL;
  int highscore = 0;

  error = sqlite3_open(DB_FILE, &db);
  if (error != SQLITE_OK) {
    error = -1;
  } else {
    const char* sql = "SELECT MAX(score) FROM highscore;";
    error = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (error != SQLITE_OK) {
      error = -1;
    } else {
      if (sqlite3_step(stmt) == SQLITE_ROW) {
        highscore = sqlite3_column_int(stmt, 0);
      }
    }
  }
  if (stmt != NULL) {
    sqlite3_finalize(stmt);
  }
  if (db != NULL) {
    sqlite3_close(db);
  }
  if (error != 0) {
    highscore = error;
  }
  return highscore;
}

void saveHighscoreDB(int score, int level) {
  sqlite3* db = NULL;
  sqlite3_stmt* stmt = NULL;

  if (score > 0) {
    if ((sqlite3_open(DB_FILE, &db)) == SQLITE_OK) {
      const char* sql = "INSERT INTO highscore (score, level) VALUES (?, ?);";
      if ((sqlite3_prepare_v2(db, sql, -1, &stmt, NULL)) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, score);
        sqlite3_bind_int(stmt, 2, level);
        sqlite3_step(stmt);
      }
    }
  }
  if (stmt != NULL) {
    sqlite3_finalize(stmt);
  }
  if (db != NULL) {
    sqlite3_close(db);
  }
}

void updateHighscoreDB(GameState* state) {
  if (state->info.score > state->info.high_score) {
    state->info.high_score = state->info.score;
    saveHighscoreDB(state->info.score, state->info.level);
  }
}
