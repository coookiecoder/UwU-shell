#include <builtins.hpp>

Error builtins::UwU_exit(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "invalid usage of exit" << std::endl;
	}
	exit(std::atoi(argv[0]));
	return Error(1, "call to exit() failed");
}