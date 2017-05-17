#!/bin/bash

gcc -O3 -c matrix.c -o matrix_o.o
gcc -c matrix.c -o matrix.o

gcc -O3 test.c matrix_o.o -o tmat_o -lm
gcc -O3 test.c matrix.o -o tmat -lm

