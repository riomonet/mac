#!/bin/sh

gcc -Wall -W -g3 -O0 -o ./bin/test_$1 ../mac.c pool.c  $1.c test_switchboard.c
