#include <builtins.hpp>

Error builtins::UwU_cd(int argc, char **argv) {
	if (argc != 2) {
		return {1, "invalid use of cd"};
	} else {
		if (chdir(argv[1]))
			return {1, std::string("cd: couldn't access the directory : ") + "\"" + argv[1] + "\""};
		return {0, ""};
	}
}