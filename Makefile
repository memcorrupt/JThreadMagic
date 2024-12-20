JAVA_HOME ?= $(shell readlink -e "$$(dirname "$$(readlink -e "$$(which javac)")")"/..)

CC := gcc
CXX := g++
CFLAGS := -Wall -O2 -DNDEBUG -fPIC -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux $(MYCFLAGS)
LDFLAGS := -shared $(MYLDFLAGS)

TARGET := libjthreadmagic.so
SRC := jthreadmagic.c
BUILD_DIR := build

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(SRC) $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRC)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)