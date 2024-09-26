#include <sys/wait.h>

#include <Shell.hpp>

int main(int argc, char **argv, char **environment_variable) {
    Shell shell(environment_variable);

    char *line = readline(PROMPT);

    while (line) {
		std::list<pid_t> pid_list;
        if (*line != '\0') {
			pid_list = shell.interpret(line);

			for (const auto &item: pid_list) {
				waitpid(item, nullptr, 0);
			}
		}
        free(line);
        line = readline(PROMPT);
    }

    std::cout << "clearing readline" << std::endl;

    rl_clear_history();

    std::cout << "done !" << std::endl;
}