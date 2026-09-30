CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -Iinclude

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

all: $(BUILD_DIR)/test_sort

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/test_sort: $(BUILD_DIR) $(SRC_DIR)/sort.c $(TEST_DIR)/test_sort.c
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/sort.c $(TEST_DIR)/test_sort.c

run: $(BUILD_DIR)/test_sort
	./$(BUILD_DIR)/test_sort

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
