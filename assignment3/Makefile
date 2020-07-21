
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L 

all: build

build:
	gcc $(CFLAGS) main.c instructorList.o screen_out.c keyboard_in.c network_in.c network_out.c -lpthread -o s-talk
  
run: build
	./s-talk 10000 gurshan-VirtualBox 20000

second: build
	./s-talk 20000 gurshan-VirtualBox 10000

valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all ./s-talk 10000 gurshan-VirtualBox 20000

clean:
	rm -f main 