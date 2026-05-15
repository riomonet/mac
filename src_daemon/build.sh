#!/bin/sh

gcc -W -Wall -O0 -g3 -o bin/mcds.out ./switchboard/pool.c ./switchboard/switchboard.c mac.c
