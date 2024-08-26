#include <Shell.hpp>
#include <readline/readline.h>

int main(int argc, char **argv, char **environment_variable) {
    Shell shell(environment_variable);

    char *line = readline(PROMPT);

    while (line) {
        if (*line != '\0')
            shell.interpret(line);
        free(line);
        line = readline(PROMPT);
    }
}