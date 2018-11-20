#
# Created on Mon Nov 19 2018
# Author: Artur Balanuta
# Copyright (c) 2018 Carnegie Mellon University
#

CC=gcc
CFLAGS += -pthread -g -O3
LDFLAGS= -pthread

all: simple simple_1th

simple:
	$(CC) -c $(CFLAGS) -o simple.o simple.c
	$(CC) $(LDFLAGS) -o simple.x ./simple.o

simple_1th:
	$(CC) -c -O0 -o simple_1th.o simple_1th.c
	$(CC) -o simple_1th.x ./simple_1th.o


clean:
	-rm *.o
	-rm simple.x
	-rm simple_1th.x