#ifndef UWU_SHELL_SHELL_HPP
#define UWU_SHELL_SHELL_HPP

#include <string>
#include <list>
#include <vector>
#include <array>

#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

#include <Command.hpp>

class Command;

#define PROMPT "\
╭─ =UwU= shell\n\
╰─> "

enum quote_state {
    NO_QUOTE = 0,
    SIMPLE_QUOTE = 1,
    DOUBLE_QUOTE = 2
};

enum quote_state update_quote_state(char item, enum quote_state quote_state);

class Shell {
private:
    int												environment_variable_size;
    std::list<std::string>							environment_variable;

public:
    explicit										Shell(char **environment_variable);
    std::list<pid_t>								interpret(const std::string& line);

private:
    std::list<std::string>							tokenize(const std::string& line);
    std::list<std::string>							expand(std::list<std::string>& token);
	std::list<std::list<Command>>					setup_command(std::list<std::string>& token, std::list<std::list<Command>>& pipe_list);
	std::vector<std::vector<std::array<int, 2>>>	open_pipe(std::vector<std::vector<std::array<int, 2>>>& pipe_fd, std::list<std::list<Command>>& pipe_list);
	std::string										get_env(const std::string& key);
};


#endif //UWU_SHELL_SHELL_HPP
