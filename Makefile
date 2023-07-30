# To do:
# - I'd like to have a single makefile for the whole project, but that might not be simple

all: bin/dash_can_cli

bin/dash_can_cli: lib/network.a cli/dash_can_cli.cpp
	mkdir -p bin
	g++ cli/dash_can_cli.cpp lib/network.a -o bin/dash-can-cli -I network/

lib/network.a:
	make -C network

clean:
	rm -r lib
	rm -r bin
	rm -r obj

rebuild:
	make clean
	make all