# Makefile

all:
	cmake -S . -B build
	cmake --build build

run: all
	./bin/sortui

clean:
	rm -rf build
