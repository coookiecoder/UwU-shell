#include <Shell.hpp>

enum quote_state update_quote_state(char item, enum quote_state quote_state) {
    if (item == '\'' && quote_state == SIMPLE_QUOTE)
        return NO_QUOTE;
    if (item == '\"' && quote_state == DOUBLE_QUOTE)
        return NO_QUOTE;
    if (item == '\'' && quote_state == NO_QUOTE)
        return SIMPLE_QUOTE;
    if (item == '\"' && quote_state == NO_QUOTE)
        return DOUBLE_QUOTE;
    return quote_state;
}

Shell::Shell(char **environment_variable) {
    this->environment_variable_size = 0;

    while (environment_variable[this->environment_variable_size]) {
        this->environment_variable.emplace_back(environment_variable[this->environment_variable_size++]);
    }
}

std::list<pid_t> Shell::interpret(const std::string& line) {
	add_history(line.c_str());

    std::list<std::string> token = Shell::tokenize(line);

	std::list<std::list<Command>> pipe_list;
	pipe_list.emplace_back();
	pipe_list.back().emplace_back();

	Shell::expand(token);
	Shell::setup_command(token, pipe_list);

	std::vector<std::vector<std::array<int, 2>>> pipe_fd;

	Shell::open_pipe(pipe_fd, pipe_list);

	std::list<pid_t> pid_list;

	try {
		int pipe_index = 0;
		int command_index = 0;

		for (auto &command_list: pipe_list) {
			command_index = 0;
			for (auto &command: command_list) {
				command.set_redirection();
				if (command_list.size() > 1) {
					if (command_index == 0) {
						command.set_pipe(0, pipe_fd[pipe_index][command_index], pipe_fd[pipe_index][command_index]);
					} else if (command_index < command_list.size() - 1) {
						command.set_pipe(1, pipe_fd[pipe_index][command_index - 1], pipe_fd[pipe_index][command_index]);
					} else if (command_index == command_list.size() - 1) {
						command.set_pipe(2, pipe_fd[pipe_index][command_index - 1], pipe_fd[pipe_index][command_index - 1]);
					}
				}
				command.purge_quote();
				command.purge_quote_binary();
				pid_list.push_back(command.execute(this->environment_variable, pipe_fd));
				command_index++;
			}
			pipe_index++;
		}
	} catch (std::runtime_error& error) {
		std::cerr << error.what() << std::endl;
	}

	for (const auto &item: pipe_fd) {
		for (auto &pipe: item) {
			close(pipe[0]);
			close(pipe[1]);
		}
	}

    return pid_list;
}

std::list<std::string> Shell::tokenize(const std::string &line) {
    std::list<std::string> token;
	enum quote_state quote_state = NO_QUOTE;

    int token_start = 0;
    int token_end = 0;

    for (const auto &item: line) {
        quote_state = update_quote_state(item, quote_state);
        if ((item == ' ' || item == '|' || item == ';') && quote_state == NO_QUOTE && token_start != token_end) {
            token.emplace_back(line.substr(token_start, token_end - token_start));
			if (item == '|')
				token.emplace_back("|");
			else if (item == ';')
				token.emplace_back(";");
            token_start = token_end + 1;
        } else if (item == ' ' && quote_state == NO_QUOTE) {
			token_start++;
		}
        token_end++;
    }

    token.emplace_back(line.substr(token_start, token_end - token_start));

    return token;
}

std::list<std::string> Shell::expand(std::list<std::string>& token) {
	size_t env_pos_start = 0;
	size_t env_pos_end = 0;

	for (auto &item: token) {
		if (item[0] == '\'')
			continue;
		env_pos_start = item.find('$');
		while (env_pos_start != std::string::npos) {
			env_pos_end = ++env_pos_start;
			while (item[env_pos_end] != ' ' && item[env_pos_end] != '$' && item[env_pos_end])
				env_pos_end++;
			std::string key = item.substr(env_pos_start, env_pos_end - env_pos_start);
			std::string value = this->get_env(key);
			if (value.empty()) {
				item.erase(env_pos_start - 1, env_pos_end - env_pos_start + 1);
				env_pos_start--;
			}
			else
				item.replace(env_pos_start - 1, env_pos_end - env_pos_start + 1, value);
			env_pos_start = item.find('$', env_pos_start);
		}
	}
	return (token);
}

std::list<std::list<Command>> Shell::setup_command(std::list<std::string>& token, std::list<std::list<Command>>& pipe_list) {
	for (const auto &item: token) {
		if (item.empty())
			continue;
		if (item == ";") {
			pipe_list.emplace_back();
			pipe_list.back().emplace_back();
		} else if (item == "|") {
			pipe_list.back().emplace_back();
		} else if (pipe_list.back().back().is_binary_set()) {
			pipe_list.back().back().set_binary(item);
		} else {
			pipe_list.back().back().add_argv(item);
		}
	}
	return (pipe_list);
}


std::vector<std::vector<std::array<int, 2>>> Shell::open_pipe(std::vector<std::vector<std::array<int, 2>>>& pipe_fd, std::list<std::list<Command>>& pipe_list) {
	for (auto &command_list: pipe_list) {
		if (command_list.size() == 1)
			continue;
		else {
			pipe_fd.emplace_back();
			for (int i = 0; i < command_list.size() - 1; ++i) {
				pipe_fd.back().emplace_back();
				if (pipe(pipe_fd.back().back().data()) == -1)
					throw std::runtime_error("pipe creation failed");
			}
		}
	}
	return (pipe_fd);
}

std::string Shell::get_env(const std::string &key) {
	for (const auto &item: this->environment_variable) {
		size_t equal = item.find('=');
		if (equal != std::string::npos) {
			std::string key_env = item.substr(0, equal);
			if (key == key_env)
				return (item.substr(equal + 1));
		} else {
			if (item == key)
				return {};
		}
	}
	return {};
}
