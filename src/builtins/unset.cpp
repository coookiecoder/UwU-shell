#include <builtins.hpp>

Error builtins::UwU_unset(int argc, char **argv, std::list<std::string>& env) {
	for (int idx = 1; idx < argc; ++idx) {
		env.remove(argv[idx]);
	}

	return {0, ""};
}