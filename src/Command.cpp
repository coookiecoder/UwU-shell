#include <Command.hpp>

static
bool is_redirection(const std::string& binary) {
	if (binary[0] == '\"' || binary[0] == '\'')
		return false;
	if (binary == ">" || binary == ">>" || binary == "<" || binary == "<<")
		return true;
	if (binary[0] == '>' || binary[0] == '<')
		return true;
	return false;
}

Command::Command() {
	binary = "";
}

void Command::set_binary(std::string new_binary) {
	if (!this->binary.empty())
		throw std::runtime_error("binary already set");
	this->binary = std::move(new_binary);
}

void Command::add_argv(const std::string& argv_to_add) {
	this->argv.push_back(argv_to_add);
}

void Command::purge_quote() {
	for (auto &token: this->argv) {
		if (token == "\"\"" || token == "\'\'") {
			token = "";
			continue ;
		}

		int position = 0;
		enum quote_state quote_state = NO_QUOTE;

		for (const auto &item: token) {
			quote_state = update_quote_state(item, quote_state);
			if (item == '\"' && (quote_state == DOUBLE_QUOTE || quote_state == NO_QUOTE)) {
				token.erase(position, 1);
			}
			if (item == '\'' && (quote_state == SIMPLE_QUOTE || quote_state == NO_QUOTE)) {
				token.erase(position, 1);
			}
			position++;
		}

		if (quote_state == DOUBLE_QUOTE)
			throw std::runtime_error("invalid syntax missing \"");
		if (quote_state == SIMPLE_QUOTE)
			throw std::runtime_error("invalid syntax missing \'");
	}
}

void Command::purge_quote_binary() {
	int position = 0;
	enum quote_state quote_state = NO_QUOTE;

	for (const auto &item: this->binary) {
		quote_state = update_quote_state(item, quote_state);
		if (item == '\"' && (quote_state == DOUBLE_QUOTE || quote_state == NO_QUOTE)) {
			this->binary.erase(position, 1);
		}
		if (item == '\'' && (quote_state == SIMPLE_QUOTE || quote_state == NO_QUOTE)) {
			this->binary.erase(position, 1);
		}
		position++;
	}

	if (quote_state == DOUBLE_QUOTE)
		throw std::runtime_error("invalid syntax missing \"");
	if (quote_state == SIMPLE_QUOTE)
		throw std::runtime_error("invalid syntax missing \'");
}

void Command::set_redirection() {
	bool next_item_redirection_output = false;
	bool next_item_redirection_input = false;

	std::list<int> item_to_remove;
	int index = 0;

	if (is_redirection(this->binary))
		throw (std::runtime_error("excepted a string, found a redirection"));

	for (const auto &item: this->argv) {
		if (next_item_redirection_output) {
			output_file = item;
			item_to_remove.push_back(index++);
			next_item_redirection_output = false;
			if (output_fd != 1)
				close(output_fd);
			output_fd = open(output_file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0655);
			continue;
		} else if (next_item_redirection_input) {
			input_file = item;
			item_to_remove.push_back(index++);
			next_item_redirection_input = false;
			if (input_fd != 0)
				close(input_fd);
			input_fd = open(input_file.c_str(), O_RDONLY);
			continue;
		} else if (item == ">") {
			next_item_redirection_output = true;
			output_append = false;
		} else if (item == ">>") {
			next_item_redirection_output = true;
			output_append = true;
		} else if (item == "<") {
			next_item_redirection_input = true;
		} else if (item[0] == '>' && item[1] != '>') {
			output_file = item.c_str() + 1;
			if (output_fd != 1)
				close(output_fd);
			output_fd = open(output_file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0655);
			output_append = false;
		} else if (item[0] == '>' && item[1] == '>' && item[2] != '>') {
		 	output_file = item.c_str() + 2;
			 if (output_fd != 1)
				close(output_fd);
			output_fd = open(output_file.c_str(), O_CREAT | O_WRONLY, 0655);
			output_append = true;
		} else if (item[0] == '<' && item[1] != '<') {
			input_file = item.c_str() + 1;
			if (input_fd != 0)
				close(input_fd);
			input_fd = open(input_file.c_str(), O_RDONLY);
		} else if (is_redirection(item)) {
			throw (std::runtime_error("excepted a string, found a redirection"));
		} else {
			index++;
			continue;
		}
		item_to_remove.push_back(index++);
	}

	int removed_element = 0;

	for (auto &item: item_to_remove) {
		auto it = this->argv.begin();
    	std::advance(it, item - removed_element++);
    	this->argv.erase(it);
	}
}

void Command::set_pipe(int mode, const std::array<int, 2> &fds, const std::array<int, 2>& fds_2) {
	if (mode == 0) {
		if (output_fd != 1)
			close(output_fd);
		output_fd = fds[1];
		output_fd_pipe = fds[0];
	}
	if (mode == 2) {
		if (input_fd != 0)
			close(input_fd);
		input_fd = fds[0];
		input_fd_pipe = fds[1];
	}
	if (mode == 1) {
		if (output_fd != 1)
			close(output_fd);
		output_fd = fds_2[1];
		output_fd_pipe = fds_2[0];

		if (input_fd != 0)
			close(input_fd);
		input_fd = fds[0];
		input_fd_pipe = fds[1];
	}
}

int Command::execute(std::list<std::string>& env, const std::vector<std::vector<std::array<int, 2>>>& pipe_fd) {
	Error error(0, "");

	char ** argv_exec = list_to_char_argv(this->binary, this->argv);

	if (this->binary == "exit")
		error = builtins::UwU_exit(static_cast<int>(this->argv.size() + 1), argv_exec);
	else if (this->binary == "cd")
		error = builtins::UwU_cd(static_cast<int>(this->argv.size() + 1), argv_exec);
	else if (this->binary == "export")
		error = builtins::UwU_export(static_cast<int>(this->argv.size() + 1), argv_exec, env);
	else if (this->binary == "unset")
		error = builtins::UwU_unset(static_cast<int>(this->argv.size() + 1), argv_exec, env);
	else {
		int pid = fork();

		if (pid == 0) {
			if (input_fd != 0) {
				dup2(input_fd, 0);
			}

			if (output_fd != 1) {
				dup2(output_fd, 1);
			}

			for (const auto &item: pipe_fd) {
				for (auto &pipe: item) {
					close(pipe[0]);
					close(pipe[1]);
				}
			}

			char **envp = list_to_char(env);

			execvpe(this->binary.c_str(), argv_exec, envp);

			std::cout << "UwU-shell: command not found : \"" << this->binary << "\"" << std::endl;
			delete[] envp;
			exit(1);
		}

		delete[] argv_exec;

		return (pid);
	}

	if (error.get_error_code())
		std::cout << "UwU-shell: " << error.get_error() << std::endl;

	delete[] argv_exec;

	return (0);
}

bool Command::is_binary_set() {
	return (this->binary.empty());
}

std::ostream &Command::operator<<(std::ostream &os) {
	os << binary << " ";
	for (const auto &arg: argv) {
		os << arg << " ";
	}
	return os;
}

const std::string &Command::get_binary() const {
	return this->binary;
}

const std::list<std::string> &Command::get_argv() const {
	return this->argv;
}

std::ostream& operator<<(std::ostream &os, const Command& cmd) {
	os << "binary : " << cmd.get_binary() << " ";
	os << "argv : ";
	for (const auto &arg: cmd.get_argv()) {
		os << arg << " ";
	}
	return os;
}

[[nodiscard]]
char ** list_to_char_argv(const std::string& bin, const std::list<std::string>& list) {
	char ** argv = new char*[list.size() + 2];
	int idx = 0;

	argv[idx++] = const_cast<char *>(bin.c_str());

	for (const auto &item: list) {
		argv[idx++] = const_cast<char *>(item.c_str()); // yes I hate it too
	}
	argv[idx] = nullptr;
	return argv;
}

[[nodiscard]]
char ** list_to_char(const std::list<std::string>& list) {
	char ** char_list = new char*[list.size() + 1];
	int idx = 0;

	for (const auto &item: list) {
		char_list[idx++] = const_cast<char *>(item.c_str()); // yes I hate it too
	}
	char_list[idx] = nullptr;
	return char_list;
}
