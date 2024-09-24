#include <builtins.hpp>

Error builtins::UwU_unset(int argc, char **argv, std::list<std::string> env, std::string path) {
	for (int idx = 1; idx < argc; ++idx) {
		env.remove(argv[idx]);
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