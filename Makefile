CC = gcc

# __USE_MINGW_ANSI_STDIO : sous Windows (MinGW), permet a printf d'accepter %zu.
# Sans effet sous Linux.
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -Iinclude -D__USE_MINGW_ANSI_STDIO=1
LDLIBS = -lm

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

# Sous Windows, gcc ajoute .exe : la cible doit porter ce nom, sinon make recompile a chaque fois.
ifeq ($(OS),Windows_NT)
EXE = .exe
endif

all: $(BUILD_DIR)/test_sort$(EXE) $(BUILD_DIR)/benchmark$(EXE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# "| $(BUILD_DIR)" : le dossier doit exister, mais sa date ne force pas de recompilation.
$(BUILD_DIR)/test_sort$(EXE): $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(TEST_DIR)/test_sort.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(TEST_DIR)/test_sort.c $(LDLIBS)

$(BUILD_DIR)/benchmark$(EXE): $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(SRC_DIR)/benchmark.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(SRC_DIR)/benchmark.c $(LDLIBS)

check: $(BUILD_DIR)/test_sort$(EXE)
	./$(BUILD_DIR)/test_sort$(EXE)

# Le benchmark n'est lance que si les tests passent.
bench: check $(BUILD_DIR)/benchmark$(EXE)
	./$(BUILD_DIR)/benchmark$(EXE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all check bench clean
