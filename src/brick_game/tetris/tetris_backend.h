/**
 * @file tetris_backend.h
 * @brief Заголовочный файл внутренней логики игры Tetris
 *
 * Содержит определения констант, структур данных и функций для реализации
 * игровой механики, управления состоянием и работы с данными игры.
 */
#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include "tetris_lib.h"

#define DB_FILE                                                   \
  "tetris_highscore.db" /**< Имя файла базы данных для рекордов \
                         */
/* Битовые маски для представления фигур тетрамино (4x4) */
#define SHAPE_I 0x4444 /**< Маска для I-фигуры (палка) */
#define SHAPE_J 0x44C0 /**< Маска для J-фигуры */
#define SHAPE_L 0x4460 /**< Маска для L-фигуры */
#define SHAPE_O 0xCC00 /**< Маска для O-фигуры (квадрат) */
#define SHAPE_S 0x6C00 /**< Маска для S-фигуры */
#define SHAPE_T 0x4E00 /**< Маска для T-фигуры */
#define SHAPE_Z 0xC600 /**< Маска для Z-фигуры */

/* Игровые константы */
#define ROWS 21       /**< Количество строк игрового поля (включая невидимые) */
#define COLMS 11      /**< Количество столбцов игрового поля */
#define BRICK_ROW 4   /**< Размер фигуры в строках (тетрамино 4x4) */
#define BRICK_COLM 4  /**< Размер фигуры в столбцах (тетрамино 4x4) */
#define MASKS 7       /**< Количество доступных фигур */
#define DEFAULT_LVL 1 /**< Начальный уровень игры */
#define DEFAULT_SPEED 1000 /**< Начальная скорость игры (мс) */
#define FOR_UP_LVL \
  600 /**< Количество очков для перехода на следующий уровень */
#define START_POS_X COLMS / 2 /**< Стартовая позиция фигуры по X (центр) */
#define START_POS_Y 0         /**< Стартовая позиция фигуры по Y (верх) */
// #define BRICK_SIZE 4
#define WIN 11 /**< Количество линий для победы */

/**
 * @brief Перечисление состояний игры
 *
 * Определяет все возможные состояния игрового процесса
 */
typedef enum {
  STATE_START,     /**< Начальное состояние, инициализация */
  STATE_PLAY,      /**< Активный игровой процесс */
  STATE_PAUSE,     /**< Игра на паузе */
  STATE_GAME_OVER, /**< Конец игры (проигрыш) */
  STATE_WIN        /**< Победа в игре */
} GameStateType;

/**
 * @brief Структура текущей активной фигуры
 *
 * Содержит информацию о позиции и форме текущей падающей фигуры
 */
typedef struct {
  int x, y; /**< Координаты верхнего левого угла фигуры на поле */
  int shape[BRICK_ROW]
           [BRICK_COLM]; /**< Матрица 4x4, представляющая форму фигуры */
} CurrentShape;

/**
 * @brief Основная структура состояния игры
 *
 * Содержит полное состояние игрового процесса, включая информацию для
 * отрисовки, текущую фигуру, временные метки и состояние конечного автомата
 */
typedef struct {
  GameInfo_t info;        /**< Информация для отрисовки (публичная часть) */
  CurrentShape current;   /**< Текущая активная фигура */
  clock_t last_timestamp; /**< Временная метка последнего обновления */
  GameStateType state;    /**< Текущее состояние игры */
} GameState;

/**
 * @brief Возвращает указатель на глобальное состояние игры
 *
 * @return GameState* Указатель на структуру состояния игры
 *
 * @note Функция обеспечивает доступ к единому экземпляру состояния игры
 */
GameState* ptrState();

/* Функции модуля Fsm.c - Конечный автомат игры */

/**
 * @brief Перемещает текущую фигуру вниз
 * @param[in,out] state Указатель на состояние игры
 */
void moveDown(GameState* state);
/**
 * @brief Перемещает текущую фигуру вправо
 * @param[in,out] state Указатель на состояние игры
 */
void moveRight(GameState* state);
/**
 * @brief Перемещает текущую фигуру влево
 * @param[in,out] state Указатель на состояние игры
 */
void moveLeft(GameState* state);
/**
 * @brief Поворачивает текущую фигуру
 * @param[in,out] state Указатель на состояние игры
 */
void moveRotation(GameState* state);
/**
 * @brief Создает новую фигуру в верхней части поля
 * @param[in,out] state Указатель на состояние игры
 */
void spawnNewBrick(GameState* state);
/**
 * @brief Переводит игру в начальное состояние
 * @param[in,out] state Указатель на состояние игры
 */
void goToStart(GameState* state);
/**
 * @brief Переводит игру в состояние активного gameplay
 * @param[in,out] state Указатель на состояние игры
 */
void goToPlay(GameState* state);
/**
 * @brief Переводит игру в состояние паузы
 * @param[in,out] state Указатель на состояние игры
 */
void goToPause(GameState* state);
/**
 * @brief Переводит игру в состояние завершения (проигрыш)
 * @param[in,out] state Указатель на состояние игры
 */
void goToGameOver(GameState* state);
/**
 * @brief Переводит игру в состояние победы
 * @param[in,out] state Указатель на состояние игры
 */
void goToWin(GameState* state);
/**
 * @brief Управляет изменением состояния игры на основе пользовательского ввода
 * @param[in,out] state Указатель на состояние игры
 * @param[in] action Действие пользователя
 */
void manageGameState(GameState* state, UserAction_t action);

/* Функции модуля Init_clean.c - Инициализация и очистка */

/**
 * @brief Инициализирует состояние игры
 * @param[out] state Двойной указатель на состояние игры для инициализации
 */
void initGame(GameState** state);
/**
 * @brief Очищает и освобождает ресурсы состояния игры
 * @param[in,out] state Двойной указатель на состояние игры для очистки
 */
void clearGameState(GameState** state);

/* Функции модуля Matrix.c - Работа с матрицами */

/**
 * @brief Создает динамическую матрицу заданного размера
 * @param[in] rows Количество строк
 * @param[in] colms Количество столбцов
 * @return int** Указатель на созданную матрицу
 */
int** createMatrix(int rows, int colms);
/**
 * @brief Освобождает память, занятую матрицей
 * @param[in] matrix Указатель на матрицу для удаления
 * @param[in] rows Количество строк в матрице
 */
void removeMatrix(int** matrix, int rows);

/* Функции модуля Tetris_backend.c - Основная игровая логика */

/**
 * @brief Распаковывает битовую маску в матрицу фигуры
 * @param[out] shape Матрица 4x4 для заполнения
 * @param[in] mask Битовый шаблон фигуры
 */
void unpackShape(int shape[BRICK_ROW][BRICK_COLM], unsigned short mask);
/**
 * @brief Генерирует следующую фигуру для preview
 * @param[in,out] state Указатель на состояние игры
 */
void nextBrick(GameState* state);
/**
 * @brief Подготавливает новую фигуру к началу падения
 * @param[in,out] state Указатель на состояние игры
 */
void setupNewBrick(GameState* state);
/**
 * @brief Закрепляет текущую фигуру в основном игровом поле
 * @param[in,out] state Указатель на состояние игры
 */
void holdBrick(GameState* state);
/**
 * @brief Проверяет и обрабатывает заполненные линии
 * @param[in,out] state Указатель на состояние игры
 */
void fullLine(GameState* state);
/**
 * @brief Очищает заполненную линию и сдвигает вышележащие линии
 * @param[in,out] state Указатель на состояние игры
 * @param[in] x Номер линии для очистки
 */
void cleanDownLine(GameState* state, int x);
/**
 * @brief Проверяет коллизию фигуры с границами поля и другими фигурами
 * @param[in] state Указатель на состояние игры
 * @param[in] x Смещение по X для проверки
 * @param[in] y Смещение по Y для проверки
 * @return true Есть коллизия
 * @return false Нет коллизии
 */
bool checkCollide(GameState* state, int x, int y);
/**
 * @brief Проверяет возможность поворота фигуры
 * @param[in] state Указатель на состояние игры
 * @param[in] temp Временная матрица повернутой фигуры
 * @param[in] size Размер фигуры
 * @return true Поворот возможен
 * @return false Поворот невозможен
 */
bool checkRotation(GameState* state, const int temp[BRICK_ROW][BRICK_COLM],
                   int size);
/**
 * @brief Выполняет поворот текущей фигуры
 * @param[in,out] state Указатель на состояние игры
 */
void rotation(GameState* state);
/**
 * @brief Корректирует позицию фигуры после поворота
 * @param[in,out] state Указатель на состояние игры
 * @param[in] size Размер фигуры
 */
void shiftRotation(GameState* state, int size);
/**
 * @brief Возвращает текущее время в миллисекундах
 * @return unsigned int Текущее время (мс)
 */
unsigned int getTimeMS(void);
/**
 * @brief Повышает уровень игры при достижении достаточного количества очков
 * @param[in,out] state Указатель на состояние игры
 */
void levelUp(GameState* state);

/* Функции модуля Highscore_db.c - Работа с рекордами */

/**
 * @brief Инициализирует базу данных рекордов
 * @return int Код ошибки (0 - успех)
 */
int initHighscoreDB(void);
/**
 * @brief Загружает рекорд из базы данных
 * @return int Код ошибки (0 - успех)
 */
int loadHighscoreDB(void);
/**
 * @brief Сохраняет рекорд в базу данных
 * @param[in] score Количество очков
 * @param[in] level Уровень игры
 */
void saveHighscoreDB(int score, int level);
/**
 * @brief Обновляет рекорд в базе данных при необходимости
 * @param[in] state Указатель на состояние игры
 */
void updateHighscoreDB(GameState* state);

#endif