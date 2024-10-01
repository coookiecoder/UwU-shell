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
	std::list<pid_t> pid_list;

    add_history(line.c_str());

    std::list<std::string> token = this->tokenize(line);

	std::list<Command> command_list;
	command_list.emplace_back();

	for (const auto &item: token) {
		if (item == "|" || item == ";") {
			command_list.emplace_back();
			continue;
		} else if (command_list.back().is_binary_set()) {
			command_list.back().set_binary(item);
		} else {
			command_list.back().add_argv(item);
		}
	}

	try {
		for (auto &item: command_list) {
			item.set_redirection();
			item.purge_quote();
			pid_list.push_back(item.execute(this->environment_variable));
		}
	} catch (std::runtime_error& error) {
		std::cerr << error.what() << std::endl;
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