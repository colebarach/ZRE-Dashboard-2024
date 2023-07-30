# To do:
# - I'd like to have a single makefile for the whole project, but that might not be simple

all: test-can-cli

test-can-cli: network
	g++ testing/test_can_cli.cpp build/network.a -o build/test-can-cli -I network/

network:
	make -C network