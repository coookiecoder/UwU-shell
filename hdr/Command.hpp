#ifndef UWU_SHELL_COMMAND_HPP
#define UWU_SHELL_COMMAND_HPP

#include <string>
#include <list>
#include <stdexcept>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>

#include <Shell.hpp>
#include <builtins.hpp>

class Command {
	private:
		std::string										binary;
		std::list<std::string>							argv;
		std::string										output_file;
		int												output_fd = 1;
		int												output_fd_pipe = -1;
		std::string										input_file;
		int 											input_fd = 0;
		int												input_fd_pipe = -1;
		bool											output_append = false;

	public:
		Command();

		void											set_binary(std::string new_binary);
		void											add_argv(const std::string& argv_to_add);
		void											set_redirection();
		void											set_pipe(int mode, const std::array<int, 2>& fds, const std::array<int, 2>& fds_2);
		void											purge_quote();
		int												execute(std::list<std::string>& env, const std::vector<std::vector<std::array<int, 2>>>& pipe_fd);

		bool											is_binary_set();

		[[nodiscard]] const std::string&				get_binary() const;
		[[nodiscard]] const std::list<std::string>&		get_argv() const;

		std::ostream&					operator<<(std::ostream &os);
};

std::ostream& operator<<(std::ostream &os, const Command& cmd);

char ** list_to_char_argv(const std::string& bin, const std::list<std::string>&list);
char ** list_to_char(const std::list<std::string>&list);

#endif