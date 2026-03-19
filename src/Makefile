CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -Werror
#CFLAGS=-std=c11 -Wall -Wextra -Werror -Wno-unused-function -D_POSIX_C_SOURCE=199309L

SRC_FRONT=gui/cli/*.c
SRC_BACK=brick_game/tetris/*.c
SRC_TEST=tests/test_tetris.c
SRC_MAIN=tetris.c

INSTALL_DIR=tetris_game
DOXYFILE=Doxyfile
DOC_DIR=documentation
DIST_DIR=tetris_dist
DIST_NAME=C7_BrickGame_v1.0

VALFLAGS = --tool=memcheck --leak-check=yes --leak-check=full --show-leak-kinds=all --track-origins=yes -s

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CLIBS=-lcheck -lm -lsubunit -lpthread -lncurses -lsqlite3
#	CLIBS=-lcheck -lm -lsubunit -lpthread -lncurses -lsqlite3 -lrt
endif

ifeq ($(UNAME_S), Darwin)
	CLIBS=-lcheck -lm -lpthread -lncurses -lsqlite3
endif

all: libtetris.a tetris

libtetris.a:
	@$(CC) $(CFLAGS) -c $(SRC_BACK)
	@ar rc libtetris.a *.o
	@ranlib libtetris.a
	@rm -f *.o

tetris: libtetris.a
	@$(CC) $(CFLAGS) $(SRC_MAIN) $(SRC_FRONT) libtetris.a -o tetris $(CLIBS)

install: tetris
	@mkdir -p $(INSTALL_DIR)
	@cp tetris $(INSTALL_DIR)/s21_tetris
	@echo "|-- The game \"Tetris\" INSTALLED --|"

uninstall:
	@if [ -d "$(INSTALL_DIR)" ]; then \
	rm -rf $(INSTALL_DIR) && \
	echo "|-- The game \"Tetris\" UNINSTALLED --|"; \
	else \
	echo "|-- Directory \"$(INSTALL_DIR)\" doesn't exist --|"; \
	fi

dvi:
	@mkdir -p $(DOC_DIR)
	@doxygen -q $(DOXYFILE)
	@if command -v open > /dev/null 2>&1; then open $(DOC_DIR)/html/index.html; \
	elif command -v xdg-open > /dev/null 2>&1; then xdg-open $(DOC_DIR)/html/index.html; \
	else echo "Open $(DOC_DIR)/html/index.html manually in your browser."; fi

dist: dvi
	@mkdir -p $(DIST_DIR)
	@tar -czf $(DIST_DIR)/$(DIST_NAME).tar.gz --exclude=$(DIST_DIR) \
	--exclude=*.o --exclude=*.a --exclude=*.gcda --exclude=*.gcno --exclude=*.dSYM \
	--exclude=*.db --exclude=tetris --exclude=test \
	.
	@echo "|-- The project distribution has been created --|"

test: libtetris.a
	@$(CC) $(CFLAGS) $(SRC_TEST) libtetris.a -o test $(CLIBS)
	@./test

gcov_report: libtetris.a
	@mkdir -p logs
	@$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage libtetris.a \
	$(SRC_TEST) $(SRC_BACK) -o logs/gcov_report $(CLIBS)
	@./logs/gcov_report
	@gcovr --html-details --gcov-exclude $(SRC_TEST) -o logs/report.html
	@open logs/report.html

lcov_rep: libtetris.a
	@mkdir -p logs_lcov
	@$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage libtetris.a \
	$(SRC_TEST) $(SRC_BACK) -o logs_lcov/gcov_report $(CLIBS)
	@./logs_lcov/gcov_report
	@lcov -t "Tests coverage" -o logs_lcov/gcov_test.info -c -d .
	@genhtml -o report logs_lcov/gcov_test.info
	@open report/index.html

clean:
	@rm -rf *.o *.a *.db tetris test logs logs_lcov \
	*.gcda *.gcno coverage.info gcov_test.info gcov_test report/ \
	$(DIST_DIR) $(DOC_DIR)
	@echo "|-- All generated files, including documentation, removed --|"

rebuild: clean all
	
clang:
	@cp ../materials/linters/.clang-format .clang-format
	@clang-format -i brick_game/tetris/*.c brick_game/tetris/*.h
	@clang-format -i gui/cli/*.c gui/cli/*.h
	@clang-format -i tests/*.c
	@clang-format -i *.c *.h
	@rm -f .clang-format
	@echo "|-- Verified for compliance with Google Style --|"

cpp:
	@cppcheck --enable=all --std=c11 $(SRC_BACK) $(SRC_FRONT) $(SRC_MAIN) $(SRC_TEST) \
	--check-level=exhaustive \
	--disable=information --suppress=missingIncludeSystem \
	--suppress=staticFunction \
	--suppress=missingInclude --suppress=checkersReport src

valgrind: clean test
	@valgrind $(VALFLAGS) ./test > valgrind.txt 2>&1
	@echo "|-- Valgrind report saved in valgrind.txt --|"
