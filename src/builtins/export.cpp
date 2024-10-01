#include <builtins.hpp>

static
bool env_valid (const std::string& env) {
	if (std::all_of(env.begin(), env.end(), [](char item) { return isalnum(item) || item == '='; })) {
		return true;
	} else {
		return false;
	}
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