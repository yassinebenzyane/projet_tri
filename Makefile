CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -O2 -Iinclude
LDLIBS = -lm

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build


all: $(BUILD_DIR)/test_sort $(BUILD_DIR)/benchmark


$(BUILD_DIR):

	mkdir -p $(BUILD_DIR)


$(BUILD_DIR)/test_sort: $(BUILD_DIR) $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(TEST_DIR)/test_sort.c

	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(TEST_DIR)/test_sort.c $(LDLIBS)


$(BUILD_DIR)/benchmark: $(BUILD_DIR) $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(SRC_DIR)/benchmark.c

	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/sort.c $(SRC_DIR)/generators.c $(SRC_DIR)/benchmark.c $(LDLIBS)


check: $(BUILD_DIR)/test_sort

	./$(BUILD_DIR)/test_sort


bench: $(BUILD_DIR)/benchmark

	./$(BUILD_DIR)/benchmark


clean:

	rm -rf $(BUILD_DIR)


.PHONY: all check bench clean
