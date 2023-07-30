CC := g++
CC_FLAGS := -Wall

ARC := ar
ARC_FLAGS := -rv

LIB_DIR := ../lib
OBJ_DIR := ../obj

SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))

LIB_FLAGS := -lpthread

TARGET := $(LIB_DIR)/network.a

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(LIB_DIR)
	$(ARC) $(ARC_FLAGS) $@ $^

$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CC) $(CC_FLAGS) -c $^ -o $@