#!/bin/bash

gcc parse_dfa.c 
./a.out 
gcc -c delta.c
gcc -c tm_lib.c
gcc -D DELTA -o tm.out tm_lib.o delta.o tm.c
./tm.out 
echo done