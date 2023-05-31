#
# makefile for image and color utilities C library
#
#

CC = gcc -std=gnu99
FLAGS = -Wall
INCLUDES =

crackParallel: crackParallel.c sha256.o utils.o
	$(CC) $(FLAGS) $(INCLUDES) crackParallel.c sha256.o utils.o -o crackParallel -lpthread

crackOpenMP: crackParallel-openmp.c sha256.o utils.o
	$(CC) $(FLAGS) $(INCLUDES) crackParallel-openmp.c sha256.o utils.o -o crackOpenMP -fopenmp

crackSequential: crackSequential.c sha256.o utils.o
	$(CC) $(FLAGS) $(INCLUDES) crackSequential.c sha256.o utils.o -o crackSequential

sha256.o: sha256.c sha256.h
	$(CC) $(FLAGS) $(INCLUDES) -c sha256.c -o sha256.o

utils.o: utils.c utils.h
	$(CC) $(FLAGS) $(INCLUDES) -c utils.c -o utils.o

clean:
	rm -f *~ *.o
