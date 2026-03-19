#include "tetris_backend.h"

int** createMatrix(int rows, int colms) {
  int** matrix = {0};
  if (rows > 0 && colms > 0) {
    bool flg = true;
    matrix = (int**)calloc(rows, sizeof(int*));
    if (!matrix) flg = false;
    int j = 0;
    for (int i = 0; i < rows && flg; i++, j++) {
      matrix[i] = (int*)calloc(colms, sizeof(int));
    }
    if (!flg) {
      for (int i = 0; i < j; i++) {
        free(matrix[i]);
      }
      free(matrix);
    }
  }
  return matrix;
}

void removeMatrix(int** matrix, int rows) {
  if (matrix != NULL) {
    for (int i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}