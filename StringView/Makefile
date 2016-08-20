DEST_DIR=../lib
LIB_DIR=lib
SRC_DIR=src
BIN_DIR=bin
INC_DIR=inc

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SOURCES))

TARGET=$(LIB_DIR)/libStringView.a

CPPFLAGS=-Wall -Werror -std=c++11 -I$(INC_DIR)

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	mkdir -p $(LIB_DIR)
	ar crf $@ $^

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p $(BIN_DIR)
	g++ $(CPPFLAGS) $^ -c -o $@

clean:
	rm -r -f bin/*.o
	rm -r -f lib/*.a

