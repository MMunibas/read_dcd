#################################################################
########################   MakeVars   ###########################
#################################################################

#CXX=g++

CXX_OPT= -std=c++0x -I "./include" -Wall -Wextra -O2

LD_LIB=

LD_OPT=

MKDIR=mkdir -p ./obj

TARGET=read_dcd

SRC=$(wildcard ./src/*.cpp)

OBJ=$(patsubst ./src/%.cpp,./obj/%.o,$(SRC))

#################################################################
########################   Makefile   ###########################
#################################################################

all:$(TARGET)
	@echo "Compilation Success"

$(TARGET):Makefile

./obj/%.o:./src/%.cpp
	@$(MKDIR)
	$(CXX) $(CXX_OPT) -c $< -o $@

$(TARGET):$(OBJ)
	$(CXX) $(CXX_OPT) $(LD_LIB) $(OBJ) -o $@ $(LD_OPT)

clean:
	rm -f $(TARGET) ./obj/*.o
