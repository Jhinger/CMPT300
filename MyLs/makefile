
TARGET = gcc main.c information.c search.c -o myls -Wall

all: build

build:
	$(TARGET)
	
clean:
	rm -f myls

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./myls -ilR .