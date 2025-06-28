CC := gcc
CFLAGS := -Wall -Wextra -o3

SRC_DIR := src
INC_DIR := inc
BUILD_DIR := build
C_MATRIX_FLOAT := lib/c-matrix-float/

VPATH := $(SRC_DIR):$(C_MATRIX_FLOAT)

TARGET := $(BUILD_DIR)/example

SOURCES := matrix.c \
           kf_altitude.c \
           example.c 

HEADERS := $(INC_DIR)/kf_altitude.h \
           $(C_MATRIX_FLOAT)/matrix.h \
           $(INC_DIR)/kf_altitude_constants.h 

OBJECTS := $(addprefix $(BUILD_DIR)/,$(SOURCES:.c=.o))

.PHONY: all
all: $(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@echo "Created directory: $(BUILD_DIR)"

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking files..."
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c $(HEADERS) | $(BUILD_DIR)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(C_MATRIX_FLOAT) -c $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning up compiled files and build directory..."
	@rm -f $(TARGET) $(OBJECTS) 
	@rmdir --ignore-fail-on-non-empty $(BUILD_DIR) 2>/dev/null || true 
