#include <builtins.hpp>

static
bool env_valid (std::string env) {
	for (const auto &item: env) {
		if (isalnum(item) || item == '=')
			continue;
		else
			return false;
	}
	return true;
}

Error builtins::UwU_export(int argc, char **argv, std::list<std::string>& env) {
	if (argc == 1) {
		for (const auto &item: env) {
			std::cout << item << std::endl;
		}
	}

	for (int idx = 1; idx < argc; ++idx) {
		if (env_valid(argv[idx]))
			env.emplace_back(argv[idx]);
		else
			return {idx - 1, "invalid environment variable (see index trough return code)"};
	}

	return {0, ""};
}