# Compilator și opțiuni
CC := x86_64-w64-mingw32-g++
CFLAGS := -Wall -Wextra -Werror -static
INCLUDES := -Iinclude -I/path/to/json/includes
LDFLAGS := -lws2_32 -static

# Directoarele proiectului
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Fisierele sursa si obiect
SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

# Executabilul rezultat
TARGET := $(BIN_DIR)/ShadowToken.exe

# Regula implicită
all: $(TARGET)

# Crearea executabilului
$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compilarea fișierelor sursă în obiecte
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)/$(dir $*)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Curățarea fișierelor generate
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Recompilarea completă
rebuild: clean all

.PHONY: all clean rebuild
