CC := gcc
SRC := main.c mmath.c
FLAGS := -x c -std=c17 -Wall -Werror -Wextra -Wunreachable-code -pedantic -ggdb `pkg-config --cflags glut glew`
LIBS := -lm `pkg-config --libs glut glew`

all: main

main: ${SRC}
	${CC} ${FLAGS} -o $@ $^ ${LIBS}

