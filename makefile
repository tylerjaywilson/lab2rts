# This is the make file for lab 2 of Real Time systems
all: main

main: main.o task.o
	g++ main.cpp task.cpp -o rtslab2

clean:
	rm *o rtslab2