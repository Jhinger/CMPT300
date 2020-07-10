CFLAGS = -Wall -g -std=c99

all: build

build:
	gcc $(CFLAGS) s-talk.c instructorList.o -lpthread -o s-talk

run:
	./s-talk

valgrind:
	valgrind --leak-check=full ./s-talk

clean:
	rm -f s-talk