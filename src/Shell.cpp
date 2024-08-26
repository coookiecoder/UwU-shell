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

    quote_state = NO_QUOTE;
}

std::list<pid_t> Shell::interpret(const std::string& line) {
    std::list<std::string> token = this->tokenize(line);

    std::list<pid_t> pid_list;
    return pid_list;
}

std::list<std::string> Shell::tokenize(const std::string &line) {
    std::list<std::string> token;

    int token_start = 0;
    int token_end = 0;

    for (const auto &item: line) {
        this->quote_state = update_quote_state(item, this->quote_state);
        if (item == ' ' && quote_state == NO_QUOTE && token_start != token_end) {
            token.emplace_back(line.substr(token_start, token_end - token_start));
            token_start = token_end + 1;
        }
        token_end++;
    }

    token.emplace_back(line.substr(token_start, token_end - token_start));

    return token;
}