all: padre

padre: main.c
	gcc main.c funciones.c -o lab2 -lm -lpthread
