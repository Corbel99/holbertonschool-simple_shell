#include "shell.h"

/**
 * main - Boucle principale du simple shell
 * @ac: Nombre d'arguments (non utilisé)
 * @av: Tableau d'arguments
 * @env: Environnement système
 * Return: 0 en cas de succès
 */
int main(int ac, char **av, char **env)
{
	char *line, **argv, *cmd_path;
	pid_t pid;
	int status;

	(void)ac;

	while (1)
	{
		display_prompt();
		line = read_line();
		argv = parse_line(line);

		/* 1. Ligne vide */
		if (argv[0] == NULL)
		{
			free(line);
			free(argv);
			continue;
		}

		/* 2. Commandes built-in (exit, env) */
		if (check_builtin(argv, line, env))
		{
			free(line);
			free(argv);
			continue;
		}

		/* 3. Recherche du chemin dans le PATH */
		cmd_path = find_path(argv[0], env);

		/* 4. Si la commande n'existe pas : ERREUR + PAS DE FORK */
		if (cmd_path == NULL)
		{
			print_error(av[0], argv[0], ": not found\n");
			free(line);
			free(argv);
			continue;
		}

		/* 5. Execution uniquement si la commande existe */
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
				exit(127);
			}
		}
		else
		{
			wait(&status);
		}

		/* Nettoyage de la mémoire du tour de boucle */
		free(cmd_path);
		free(line);
		free(argv);
	}
	return (0);
}
