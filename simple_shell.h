#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

char *read_line(void);
char **parse_line(char *line);
int check_builtin(char **argv);
char *find_path(char *command, char **envp);
int execute_command(char *pathname, char **argv, char **envp);

#endif