CC=gcc
FLAGS := -x c++ -std=c++2a -Wall -Werror -Wextra -pedantic -ggdb `pkg-config --cflags glut glew`
LIBS := -lm `pkg-config --libs glut glew`

main: main.cpp
	${CC} ${FLAGS} -o $@ $^ ${LIBS}

