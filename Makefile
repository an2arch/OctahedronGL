CC=gcc
SRC=main.c mmath.c
FLAGS := -x c++ -std=c++2a -Wall -Werror -Wextra -pedantic -ggdb `pkg-config --cflags glut glew`
LIBS := -lm `pkg-config --libs glut glew`

all: main

main: ${SRC}
	${CC} ${FLAGS} -o $@ $^ ${LIBS}

