CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic

SRC_DIR := src
OBJ_DIR := build/obj
BIN_DIR := build/bin

TARGET := $(BIN_DIR)/pz
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

ifeq ($(BUILD), release)
	CXXFLAGS += -O2 -DNDEBUG
else
	CXXFLAGS += -g -O0 -DDEBUG
	BUILD := debug
endif

.PHONY: all clean debug release run help

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "[CC] $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "[LINK] $@"
	$(CXX) $^ -o $@
	@echo "Build gotov: $@"

clean:
	@echo "Brisanje build direktorijuma"
	rm -rf build/
	@echo "Očišćeno."

debug:
	$(MAKE) BUILD=debug all

release:
	$(MAKE) BUILD=release all

help:
	@echo ""
	@echo "Dostupne komande:"
	@echo "  make           - Gradi projekat (debug mod)"
	@echo "  make clean     - Briše build direktorijum"
	@echo "  make debug     - Debug build"
	@echo "  make release   - Release build"
	@echo "  make help      - Prikaz ove poruke"
	@echo ""

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)
