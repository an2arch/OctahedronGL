CC := gcc
SRC := main.c mmath.c
FLAGS := -x c -std=c17 -Wall -Werror -Wextra -Wunreachable-code -pedantic -ggdb
LIBS := -I/usr/include/ -L/usr/lib/x86_64-linux-gnu/ -lm -lGLEW -lGL -lglut

all: main

main: ${SRC}
	${CC} ${FLAGS} -o $@ $^ ${LIBS}

