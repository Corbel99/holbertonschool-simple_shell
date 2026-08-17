#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

void display_prompt(void);
char *read_line(void);
char **parse_line(char *line);
int check_builtin(char **argv);
char *find_path(char *command, char **envp);
char *get_path(char **envp);
int execute_command(char *pathname, char **argv, char **envp);

#endif