#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define DELIM " \t\r\n\a"

/* Prototypes des fonctions d'entrée et built-ins */
void display_prompt(void);
char *read_line(int *status);
char **parse_line(char *line);
int check_builtin(char **argv, char *line, char **env, int status);

/* Prototypes pour la recherche de commande */
char *get_path(char **envp);
char *find_path(char *command, char **envp);

/* Prototype pour l'exécution */
int execute_command(char *cmd_path, char **argv, char **env);

/* Gestion des erreurs (error.c) */
void print_error(char *shell_name, char *command, char *msg);

#endif /* SHELL_H */
