#!/bin/sh
make clean debug && valgrind --tool=callgrind --dump-instr=yes --simulate-cache=yes --collect-jumps=yes --log-socket=127.0.0.1 --callgrind-out-file=callgrind.out ./bin/JeuDeNim -debug 2>cerr.txt
