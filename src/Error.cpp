#include <Error.hpp>

Error::Error(int error_code, std::string error) : error_code(error_code), error(error) {

}

Error &Error::operator=(const Error & src) {
	this->error_code = src.error_code;
	this->error = src.error;

	return *this;
}

int Error::get_error_code() {
	return this->error_code;
}

std::string Error::get_error() {
	return this->error;
}