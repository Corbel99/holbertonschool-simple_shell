#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * main - simple shell that executes commands with their full path
 *
 * Return: Always 0.
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;
	size_t len = 0;
	char *cmd;
	pid_t pid;
	int status;
	char **exec_argv;

	(void)argc;
	(void)argv;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			printf("#cisfun$ ");

		if (getline(&line, &len, stdin) == -1)
			break;

		/* Extraction de la commande (élimine espaces, tabulations et \n) */
		cmd = strtok(line, " \t\n");

		/* Si la ligne est vide ou ne contient que des espaces/sauts de ligne */
		if (cmd == NULL)
			continue;

		pid = fork();

		if (pid == -1)
		{
			perror("fork");
			free(line);
			return (1);
		}

		if (pid == 0)
		{
			exec_argv = malloc(sizeof(char *) * 2);

			if (exec_argv == NULL)
			{
				perror("malloc");
				free(line);
				exit(1);
			}

			exec_argv[0] = cmd;
			exec_argv[1] = NULL;

			execve(cmd, exec_argv, envp);
			perror("execve");
			free(exec_argv);
			free(line);
			exit(1);
		}

		if (pid > 0)
			wait(&status);
	}

	free(line);
	return (0);
}
