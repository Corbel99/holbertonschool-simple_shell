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
	pid_t pid;
	int status;
	char **exec_argv;

	(void)argc;
	(void)argv;

	while (1)
	{
		printf("#cisfun$ ");

		if (getline(&line, &len, stdin) == -1)
			break;

		len = strlen(line);

		if (line[len - 1] == '\n')
		{
			line[len - 1] = '\0';
		}

		pid = fork();

		if (pid == -1)
		{
			perror("fork");
			return (1);
		}

		if (pid == 0)
		{
			exec_argv = malloc(sizeof(char *) * 2);

			if (exec_argv == NULL)
			{
				perror("malloc");
				return (1);
			}

			exec_argv[0] = line;
			exec_argv[1] = NULL;

			execve(line, exec_argv, envp);
			perror("execve");
			free(exec_argv);
			return (1);
		}

		if (pid > 0)
		{
			wait(&status);
		}
	}

	free(line);
	return (0);
}
