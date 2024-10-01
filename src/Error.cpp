#include <Error.hpp>

#include <utility>

Error::Error(int error_code, std::string error) : error_code(error_code), error(std::move(error)) {

}

Error &Error::operator=(const Error & src) = default;

int Error::get_error_code() const {
	return this->error_code;
}

std::string Error::get_error() const {
	return this->error;
}