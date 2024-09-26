#include <Command.hpp>
#include <utility>

Command::Command() {
	binary = "";
}

void Command::set_binary(std::string new_binary) {
	if (!this->binary.empty())
		throw std::runtime_error("binary already set");
	this->binary = std::move(new_binary);
}

void Command::add_argv(const std::string& argv_to_add) {
	this->argv.push_back(argv_to_add);
}

void Command::set_redirection() {
	bool next_item_redirection_output = false;
	bool next_item_redirection_input = false;

	std::list<int> item_to_remove;
	int index = 0;

	for (const auto &item: this->argv) {
		if (next_item_redirection_output) {
			output_file = item;
			item_to_remove.push_back(index++);
			next_item_redirection_output = false;
			continue;
		} else if (next_item_redirection_input) {
			input_file = item;
			item_to_remove.push_back(index++);
			next_item_redirection_input = false;
			continue;
		} else if (item[0] == '"' || item[0] == '\'') {
			index++;
			continue;
		} else if (item == ">") {
			next_item_redirection_output = true;
			output_append = false;
		} else if (item == ">>") {
			next_item_redirection_output = true;
			output_append = true;
		} else if (item == "<") {
			next_item_redirection_input = true;
		} else if (item[0] == '>' && item[1] != '>') {
			output_file = item.c_str() + 1;
			output_append = false;
		} else if (item[0] == '>' && item[1] == '>') {
		 	output_file = item.c_str() + 2;
			output_append = true;
		} else if (item[0] == '<') {
			input_file = item.c_str() + 1;
		} else {
			index++;
			continue;
		}
		item_to_remove.push_back(index++);
	}

	int removed_element = 0;

	for (auto &item: item_to_remove) {
		auto it = this->argv.begin();
    	std::advance(it, item - removed_element++);
    	this->argv.erase(it);
	}
}

bool Command::is_binary_set() {
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