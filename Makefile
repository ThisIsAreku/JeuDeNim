# Makefile
# Copyright 2013 Alexandre Boucey <alexandre.boucey@alumni.univ-avignon.fr>
FLAGS=-pthread
CXXFLAGS+=-std=c++0x -Wall -Wextra -Wno-null-conversion# -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused -Wno-shadow
LDFLAGS=-lncurses -ltinfo
NCURSES= $(shell find /usr/lib -name "libncurses.so" -print -quit 2>/dev/null)
EXEC=JeuDeNim
HEADERS_DIR=headers
SRC= $(shell find src/ -name "*.cpp")
HEADERS= $(shell find ${HEADERS_DIR}/ -name "*.h")
OBJ= $(SRC:src/%.cpp=obj/%.o)

all: CXXFLAGS+= -O3
all: init compile

debug: CXXFLAGS+= -g -pg
debug: init compile

nothreads: FLAGS=
nothreads: init compile

build: $(OBJ)
compile: $(EXEC)

format:
	astyle $(HEADERS) $(SRC)

$(EXEC): $(OBJ)
	@echo "Using ncurses @ ${NCURSES}"
	${CXX} ${FLAGS} ${LDFLAGS} -o bin/$@ $^ $(NCURSES)

obj/main.o: $(HEADERS)

obj/%.o: src/%.cpp
	-@if [ ! -d "$(shell dirname $@)" ]; then mkdir -p $(shell dirname $@); fi 
	${CXX} ${FLAGS} ${CXXFLAGS} -c -I${HEADERS_DIR} -o $@ $<

init:
	@echo "Using ${CXX}"
	${CXX} --version
	mkdir -p obj bin

.PHONY: clean mrproper

clean:
	rm -rf obj

mrproper: clean
	rm -rf bin
