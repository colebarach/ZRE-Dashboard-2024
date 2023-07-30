# To do:
# - I'd like to have a single makefile for the whole project, but that might not be simple

all: bin/test-can-cli

bin/test-can-cli: lib/network.a testing/test_can_cli.cpp
	mkdir -p bin
	g++ testing/test_can_cli.cpp lib/network.a -o bin/test-can-cli -I network/

lib/network.a:
	make -C network

clean:
	rm -r lib
	rm -r bin
	rm -r obj

rebuild:
	make clean
	make all