CXX := g++
CXXFLAGS := -O2 -s

.PHONY: all clean

all: main it-main

main: main.cpp rb-tree.hpp
	${CXX} ${CXXFLAGS} -o $@ $<

it-main: it-main.cpp interval-tree.hpp
	${CXX} ${CXXFLAGS} -o $@ $<

clean:
	rm -rf main it-main
