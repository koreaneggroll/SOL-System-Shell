all: ./src/main.cpp
	g++ ./src/main.cpp ./include/shell.cpp -o ./build/shell.bin