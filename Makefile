CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11
LIBS     := $(shell pkg-config --cflags --libs raylib) -lm

BUILD_DIR := build
TARGET    := $(BUILD_DIR)/simulation
SRC       := $(wildcard *.c lib/*.c)

all: $(TARGET)

# This line creates the directory if it does not exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean

