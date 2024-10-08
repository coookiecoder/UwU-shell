#ifndef UWU_SHELL_BUILTINS_HPP
#define UWU_SHELL_BUILTINS_HPP

#include <iostream>
#include <string>
#include <list>
#include <algorithm>

#include <unistd.h>

#include <Error.hpp>

namespace builtins {
	Error	UwU_cd(int argc, char** argv);
	Error	UwU_exit(int argc, char** argv);
	Error	UwU_export(int argc, char** argv, std::list<std::string>& env);
	Error	UwU_unset(int argc, char** argv, std::list<std::string>& env);
}

#endif
