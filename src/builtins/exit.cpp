#include <builtins.hpp>
#include <sstream>

Error builtins::UwU_exit(int argc, char **argv) {
	if (argc > 2) {
		return {1, "exit: Too many argument"};
	}
	if (argc == 1)
		exit(0);
	for (int idx = 0; argv[1][idx]; idx++) {
		if (!std::isdigit(argv[1][idx]))
			return {1, std::string("exit: Argument '") + argv[1] + "' is not a valid integer"};
	}
	std::string buffer_string(argv[1]);
	std::stringstream buffer(buffer_string);
	int exit_code;

	buffer >> exit_code;

	exit(exit_code);
}
