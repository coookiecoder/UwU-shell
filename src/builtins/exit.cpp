#include <builtins.hpp>
#include <sstream>

Error builtins::UwU_exit(int argc, char **argv) {
	if (argc != 2) {
		exit(0);
	}
	std::string buffer_string(argv[1]);
	std::stringstream buffer(buffer_string);
	int exit_code;

	buffer >> exit_code;

	exit(exit_code);
}
