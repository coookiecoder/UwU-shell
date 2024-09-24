#ifndef UWU_SHELL_COMMAND_HPP
#define UWU_SHELL_COMMAND_HPP

#include <string>
#include <list>
#include <stdexcept>
#include <iostream>

class Command {
	private:
		std::string						binary;
		std::list<std::string>			argv;
	public:
		Command();

		void							set_binary(std::string new_binary);
		void							add_argv(std::string argv_to_add);

		bool							is_binary_set(void);

		const std::string&				get_binary() const;
		const std::list<std::string>&	get_argv() const;

		std::ostream&					operator<<(std::ostream &os);
};

std::ostream& operator<<(std::ostream &os, const Command& cmd);


#endif