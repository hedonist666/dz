#!/bin/bash

gcc -c demo.c
gcc -shared -o libpastr.so -fPIC main.c -L.
gcc demo.o -L. -lpastr -I.
LD_LIBRARY_PATH="." ./a.out
