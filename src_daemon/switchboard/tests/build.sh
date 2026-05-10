#!/bin/sh

gcc -Wall -W -g3 -O0 -o ./bin/test_$1 ../arryutils.c ../pool.c ../$1.c test_$1.c
