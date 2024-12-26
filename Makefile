CC = g++

CFLAGS = `pkg-config --cflags --libs opencv4` -O3

INC = include/

TARGET = bin/FindOrientation

SRC = src/*.cpp

make: src/main.cpp
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) -I $(INC)
