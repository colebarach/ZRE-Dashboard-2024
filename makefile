# To do:
# network.a doesn't detect changes in the source. If something is changed the -B flag must be included in the make command

all: lib/network.a bin/dash-can-cli bin/dash-bms-test qt_build/dash_qt

bin/dash-can-cli: lib/network.a cli/dash_can_cli.cpp
	mkdir -p bin
	g++ cli/dash_can_cli.cpp lib/network.a -o bin/dash-can-cli -I network/

bin/dash-bms-test: lib/network.a cli/dash_bms_test.cpp
	mkdir -p bin
	g++ cli/dash_bms_test.cpp lib/network.a -o bin/dash-bms-test -I network/

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