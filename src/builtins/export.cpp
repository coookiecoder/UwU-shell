#include <builtins.hpp>

static
bool env_valid (const std::string& env) {
	int equal = 0;

	for (const auto &item: env) {
		if (item == '=')
			equal++;
		if (equal > 1)
			return false;
		if (!std::isalnum(item) && item != '=')
			return false;
	}
	return true;
}

static
bool env_exist(const std::string& env_export, std::list<std::string>& env) {
	if (env_export.find('=') == std::string::npos) {
		for (const auto &item: env) {
			if (item.find('=') != std::string::npos && item.substr(0, item.find('=')) == env_export)
				return true;
			else if (item.find('=') == std::string::npos && item == env_export)
				return true;
		}
	} else {
		for (const auto &item: env) {
			if (item.find('=') != std::string::npos && item.substr(0, item.find('=')) == env_export.substr(0, env_export.find('=')))
				return true;
			else if (item.find('=') == std::string::npos && item == env_export.substr(0, env_export.find('=')))
				return true;
		}
	}
	return false;
}

static
void update_env(const std::string& env_export, std::list<std::string>& env) {
	if (env_export.find('=') == std::string::npos) {
		for (auto &item: env) {
			if (item.find('=') != std::string::npos && item.substr(0, item.find('=')) == env_export)
				item = env_export;
			else if (item.find('=') == std::string::npos && item == env_export)
				item = env_export;
		}
	} else {
		for (auto &item: env) {
			if (item.find('=') != std::string::npos && item.substr(0, item.find('=')) == env_export.substr(0, env_export.find('=')))
				item = env_export;
			else if (item.find('=') == std::string::npos && item == env_export.substr(0, env_export.find('=')))
				item = env_export;
		}
	}
}

Error builtins::UwU_export(int argc, char **argv, std::list<std::string>& env) {
	if (argc == 1) {
		for (const auto &item: env) {
			std::cout << item << std::endl;
		}
	}

	for (int idx = 1; idx < argc; ++idx) {
		if (env_valid(argv[idx])) {
			if (env_exist(argv[idx], env))
				update_env(argv[idx], env);
			else
				env.emplace_back(argv[idx]);
		}
		else
			return {idx, "invalid environment variable (see index trough return code)"};
	}

	return {0, ""};
}