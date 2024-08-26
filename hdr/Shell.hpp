#ifndef UWU_SHELL_SHELL_HPP
#define UWU_SHELL_SHELL_HPP

#include <string>
#include <list>

#include <iostream>

#include <readline/readline.h>
#include <readline/history.h>

#define PROMPT "\
╭─UwU shell\n\
╰─>"

enum quote_state {
    NO_QUOTE = 0,
    SIMPLE_QUOTE = 1,
    DOUBLE_QUOTE = 2
};

enum quote_state update_quote_state(char item);

class Shell {
private:
    int                     environment_variable_size;
    std::list<std::string>  environment_variable;

    enum quote_state        quote_state;

public:
    explicit                Shell(char **environment_variable);
    std::list<pid_t>        interpret(const std::string& line);

private:
    std::list<std::string>  tokenize(const std::string& line);
};


#endif //UWU_SHELL_SHELL_HPP
