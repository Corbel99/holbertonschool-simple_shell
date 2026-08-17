#include "shell.h"

/**
 * execute_command - Crée un processus enfant et exécute la commande
 * @cmd_path: Chemin absolu vers la commande
 * @argv: Tableau des arguments
 * @env: Variable d'environnement
 * Return: Code de statut de sortie du processus enfant
 */
int execute_command(char *cmd_path, char **argv, char **env)
{
	pid_t pid;
	int status = 0;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}

	if (pid == 0)
	{
		if (execve(cmd_path, argv, env) == -1)
		{
			perror(argv[0]);
			exit(127);
		}
	}
	else
	{
		wait(&status);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}

	return (0);
}
