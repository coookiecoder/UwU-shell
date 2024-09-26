#ifndef UWU_SHELL_COMMAND_HPP
#define UWU_SHELL_COMMAND_HPP

#include <string>
#include <list>
#include <stdexcept>
#include <iostream>

#include <unistd.h>

#include <builtins.hpp>

class Command {
	private:
		std::string										binary;
		std::list<std::string>							argv;
		std::string										output_file;
		std::string										input_file;
		bool											output_append = false;
	public:
		Command();

		void											set_binary(std::string new_binary);
		void											add_argv(const std::string& argv_to_add);
		void											set_redirection();
		int												execute(std::list<std::string>& env);

		bool											is_binary_set(void);

		[[nodiscard]] const std::string&				get_binary() const;
		[[nodiscard]] const std::list<std::string>&		get_argv() const;

		std::ostream&					operator<<(std::ostream &os);
};

std::ostream& operator<<(std::ostream &os, const Command& cmd);

char ** list_to_char_argv(const std::string& bin, const std::list<std::string>&list);
char ** list_to_char(const std::list<std::string>&list);

#endif