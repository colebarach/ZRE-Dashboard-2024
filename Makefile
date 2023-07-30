# To do:
# - I'd like to have a single makefile for the whole project, but that might not be simple

all: build/test-can-cli

build/test-can-cli: build/network.a testing/test_can_cli.cpp
	g++ testing/test_can_cli.cpp build/network.a -o build/test-can-cli -I network/

build/network.a:
	make -C network

clean:
	rm -r build

rebuild:
	make clean
	make all