turtle: turtle.c
	gcc -std=c99 -pedantic -Wall -O3 turtle.c display.c -lSDL2 -o turtle
