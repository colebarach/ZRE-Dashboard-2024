GCC := g++

SRC :=	can_socket.cpp \
		can_dbc.cpp \
    	database.cpp \
	   	can_database.cpp

LIBS_FLAGS := -lpthread

all: test-can-cli

test-can-cli: test_can_cli.cpp $(SRC)
	$(GCC) test_can_cli.cpp -o test-can-cli $(SRC) $(LIBS_FLAGS)