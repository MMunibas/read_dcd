PKG_CFLAGS=-I ../include -D R_DCD_LIB

SRC=$(wildcard ../src/*.cpp)

all:
	R CMD SHLIB $(SRC) -o libR_DCD.so
	
clean:
	rm -f libR_DCD.so
	