#!/bin/sh

gcc -Wall -W -g3 -O0 -o test_$1 ../$1.c test_$1.c
