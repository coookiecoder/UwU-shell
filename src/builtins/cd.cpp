#include <builtins.hpp>

Error builtins::UwU_cd(int argc, char **argv) {
	if (argc != 2) {
		return Error(1, "invalid use of cd");
	} else {
		chdir(argv[0]);
		return Error(0, "");
	}
}