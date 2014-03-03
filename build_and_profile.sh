#!/bin/sh
make clean debug && valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./bin/JeuDeNim