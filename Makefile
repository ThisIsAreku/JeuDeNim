# Makefile
# Copyright 2013 Alexandre Boucey <alexandre.boucey@alumni.univ-avignon.fr>
CXXFLAGS+=-std=c++0x -W -Wall -Wextra -O3
LDFLAGS=-lncurses -ltinfo
NCURSES= $(shell find /usr/lib -name "libncurses.so" -print -quit 2>/dev/null)
EXEC=JeuDeNim
HEADERS_DIR=headers
SRC= $(shell find src/ -name "*.cpp")
HEADERS= $(shell find ${HEADERS_DIR}/ -name "*.h")
OBJ= $(SRC:src/%.cpp=obj/%.o)

all: init compile

debug: CXXFLAGS+= -g
debug: init compile

build: $(OBJ)
compile: $(EXEC)

format:
	astyle $(HEADERS) $(SRC)

$(EXEC): $(OBJ)
	@echo "Using ncurses @ ${NCURSES}"
	${CXX} ${LDFLAGS} -o bin/$@ $^ $(NCURSES)

obj/main.o: $(HEADERS)

obj/%.o: src/%.cpp
	-@if [ ! -d "$(shell dirname $@)" ]; then mkdir -p $(shell dirname $@); fi 
	${CXX} ${CXXFLAGS} -c -I${HEADERS_DIR} -o $@ $<

init:
	@echo "Using ${CXX}"
	${CXX} --version
	mkdir -p obj bin

.PHONY: clean mrproper

clean:
	rm -rf obj

mrproper: clean
	rm -rf bin
