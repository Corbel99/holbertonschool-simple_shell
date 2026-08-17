#include "shell.h"

/**
 * main - Boucle principale du simple shell
 * @ac: Nombre d'arguments (non utilisé)
 * @av: Tableau d'arguments (non utilisé)
 * @env: Environnement système
 * Return: 0 en cas de succès
 */
int main(int ac, char **av, char **env)
{
	char *line, **argv, *cmd_path;
	pid_t pid;
	int status;

	(void)ac;
	(void)av;

	while (1)
	{
		display_prompt();
		line = read_line();
		argv = parse_line(line);

		/* Gestion ligne vide */
		if (argv[0] == NULL)
		{
			free(line);
			free(argv);
			continue;
		}

		/* Gestion des built-ins */
		if (check_builtin(argv, line, env))
		{
			free(line);
			free(argv);
			continue;
		}

		/* 1. Résolution du chemin */
		cmd_path = find_path(argv[0], env);

		/* 2. RÈGLE TASK 3 : Aucun fork si la commande n'existe pas */
		if (cmd_path == NULL)
		{
			perror(argv[0]);
			free(line);
			free(argv);
			continue;
		}

		/* 3. Le fork se fait SEULEMENT si la commande existe */
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(cmd_path);
			free(line);
			free(argv);
			continue;
		}

		if (pid == 0)
		{
			if (execve(cmd_path, argv, env) == -1)
			{
				perror(argv[0]);
				free(cmd_path);
				free(line);
				free(argv);
				exit(1);
			}
		}
		else
		{
			wait(&status);
		}

		/* Libération de la mémoire allouée par find_path */
		free(cmd_path);
		free(line);
		free(argv);
	}
	return (0);
}
