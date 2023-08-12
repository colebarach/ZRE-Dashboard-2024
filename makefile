# To do:
# - I'd like to have a single makefile for the whole project, but that might not be simple

all: lib/network.a bin/dash_can_cli bin/dash-tui qt_build/dash_qt

bin/dash_can_cli: lib/network.a cli/dash_can_cli.cpp
	mkdir -p bin
	g++ cli/dash_can_cli.cpp lib/network.a -o bin/dash-can-cli -I network/

bin/dash-tui: lib/network.a tui/main.cpp
	mkdir -p bin
	g++ tui/main.cpp lib/network.a -o bin/dash-tui -I network/ -lncurses

qt_build/dash_qt:
	make -C qt

lib/network.a:
	make -C network

clean:
	rm -r lib
	rm -r bin
	rm -r obj

rebuild:
	make clean
	make all

create-vcan:
	sudo ip link add dev vcan0 type vcan
	sudo ip link set up vcan0

delete-vcan:
	sudo ip link set down vcan0
	sudo ip link delete vcan0