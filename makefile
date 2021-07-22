DIR_INC = ./include
DIR_BIN = ./bin
DIR_OBJ = ./obj
DIR_SRC = ./src

SRC = $(wildcard $(DIR_SRC)/*.cc)
OBJ = $(patsubst %.cc, $(DIR_OBJ)/%.o, $(notdir $(SRC)))

CC = g++
CPPFLAGS = -Wall -lpthread -I$(DIR_INC)
TARGET = main
BIN_TARGET = $(DIR_BIN)/$(TARGET)
.PHONY = clean

$(BIN_TARGET):$(OBJ)
	$(CC) $(OBJ) -o $@ $(CPPFLAGS)

$(DIR_OBJ)/%.o:$(DIR_SRC)/%.cc
	$(CC) -c $< -o $@ $(CPPFLAGS)

clean:
	@find $(DIR_OBJ) -name *.o -exec $(RM) {} \;