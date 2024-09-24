#include <builtins.hpp>

Error builtins::UwU_env(int argc, char **argv, std::list<std::string> env) {
	if (argc != 2)
		return Error(1, "invalid use of env");
	else {
		for (const auto &item: env) {
			std::cout << item << std::endl;
		}
		return Error(0, "");
	}
}