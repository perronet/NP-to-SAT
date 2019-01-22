#!/bin/bash

gcc parse_dfa.c 
./a.out 
gcc -c delta.c
gcc -D DELTA -o tm.out delta.o tm.c
./tm.out 
echo done