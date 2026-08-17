#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOKENS 64

/**
 * main - simple shell that executes commands with their arguments
 *
 * Return: Always 0.
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t len = 0;
	pid_t pid;
	int status, i;
	char **exec_argv;
	char *token;

	(void)argc;
	(void)argv;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#cisfun$ ");

		if (getline(&line, &len, stdin) == -1)
			break;

		exec_argv = malloc(sizeof(char *) * MAX_TOKENS);
		if (exec_argv == NULL)
		{
			perror("malloc");
			free(line);
			return (1);
		}

		/* Extraction de tous les arguments */
		i = 0;
		token = strtok(line, " \t\n");
		while (token != NULL && i < MAX_TOKENS - 1)
		{
			exec_argv[i] = token;
			i++;
			token = strtok(NULL, " \t\n");
		}
		exec_argv[i] = NULL;

		/* Ignorer les lignes vides */
		if (exec_argv[0] == NULL)
		{
			free(exec_argv);
			continue;
		}

		pid = fork();

		if (pid == -1)
		{
			perror("fork");
			free(exec_argv);
			free(line);
			return (1);
		}

		if (pid == 0)
		{
			execve(exec_argv[0], exec_argv, envp);
			perror("execve");
			free(exec_argv);
			free(line);
			exit(1);
		}

		if (pid > 0)
		{
			wait(&status);
			free(exec_argv);
		}
	}

	free(line);
	return (0);
}
