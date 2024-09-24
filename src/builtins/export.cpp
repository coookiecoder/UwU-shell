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

Error builtins::UwU_export(int argc, char **argv, std::list<std::string> env, std::string path) {
	if (argc == 1)
		return UwU_env(argc, argv, env);

	for (int idx = 1; idx < argc; ++idx) {
		if (env_valid(argv[idx]))
			env.emplace_back(argv[idx]);
		else
			return Error(idx - 1, "invalid environment variable (see index trough return code)");
	}

	char ** envp = new char*[env.size() + 1];

	int idx = 0;

	for (const auto &item: env) {
		envp[idx++] = const_cast<char *>(item.c_str()); // yes I hate it too
	}

	envp[idx] = nullptr;

	char **argv_2 = new char*[2];

	argv_2[0] = argv[0];
	argv_2[1] = nullptr;

	execvpe(path.c_str(), argv_2, envp);
	return Error(1, "did you moved/removed the UwU-shell binary ?");
}