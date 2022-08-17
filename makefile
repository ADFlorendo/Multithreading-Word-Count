# A Makefile for Program 2, CS480
# This is a sample, you may need to modify this file to make it works

PROGRAM =testmulthreads
CC = gcc
CFLAGS = -g -std=c99 

${PROGRAM}:	${PROGRAM}.o specialcountmulthreads.o
		${CC} -o ${PROGRAM} ${PROGRAM}.o specialcountmulthreads.o -lpthread -lm

specialcountmulthreads.o ${PROGRAM}.o:	count.h


clean:
		rm -f *.o ${PROGRAM}		
