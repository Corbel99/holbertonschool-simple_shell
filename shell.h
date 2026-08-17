#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DELIM " \t\r\n\a"

void display_prompt(void);
char *read_line(void);
char **parse_line(char *line);
int check_builtin(char **argv, char *line, char **env);
char *find_path(char *command, char **envp);
char *get_path(char **envp);

void print_error(char *shell_name, char *command, char *msg);

#endif