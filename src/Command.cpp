#include <Command.hpp>

Command::Command() {
	binary = "";
}

void Command::set_binary(std::string new_binary) {
	if (!this->binary.empty())
		throw std::runtime_error("binary already set");
	this->binary = new_binary;
}

void Command::add_argv(std::string argv_to_add) {
	this->argv.push_back(argv_to_add);
}

bool Command::is_binary_set(void) {
	return (this->binary.empty());
}

std::ostream &Command::operator<<(std::ostream &os) {
	os << binary << " ";
	for (const auto &arg: argv) {
		os << arg << " ";
	}
	return os;
}

const std::string &Command::get_binary() const {
	return this->binary;
}

const std::list<std::string> &Command::get_argv() const {
	return this->argv;
}

std::ostream& operator<<(std::ostream &os, const Command& cmd) {
	os << cmd.get_binary() << " ";
	for (const auto &arg: cmd.get_argv()) {
		os << arg << " ";
	}
	return os;
}