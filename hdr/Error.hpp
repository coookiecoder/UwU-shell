#ifndef UWU_SHELL_ERROR_HPP
#define UWU_SHELL_ERROR_HPP

#include <string>

class Error {
	private:
		int			error_code;
		std::string	error;
	public:
		Error(int error_code, std::string error);

		Error&operator=(const Error &);

		int			get_error_code();
		std::string get_error();
};


#endif //UWU_SHELL_ERROR_HPP
