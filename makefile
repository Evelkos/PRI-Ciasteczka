all: Projekt3.c Projekt3f.c spis.h
	gcc Projekt3f.c Projekt3.c spis.h -o a -pedantic -Wall
	./a
