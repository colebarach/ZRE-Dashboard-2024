all: test-can-database-cli

test-can-cli: test_can_cli.cpp
	g++ test_can_cli.cpp can_interface.cpp -o test_can_cli

test-db-cli: test_db_cli.cpp
	g++ test_db_cli.cpp database.cpp -o test_db_cli

test-can-dbc-cli: test_can_dbc_cli.cpp
	g++ test_can_dbc_cli.cpp can_dbc.cpp -o test_can_dbc_cli

test-can-database-cli: test_can_database_cli.cpp
	g++ test_can_database_cli.cpp can_dbc.cpp database.cpp can_socket.cpp -o test_can_database_cli -lpthread