#include "shell.h"

/**
 * main - Boucle principale du simple shell
 * @ac: Nombre d'arguments (non utilisé)
 * @av: Tableau d'arguments
 * @env: Environnement système
 * Return: Dernier code de statut d'exécution
 */
int main(int ac, char **av, char **env)
{
	char *line, **argv, *cmd_path;
	pid_t pid;
	int status = 0, last_status = 0;

	(void)ac;

	while (1)
	{
		display_prompt();
		line = read_line(&last_status); /* Transmission du statut à la fermeture */
		argv = parse_line(line);

		/* 1. Ligne vide */
		if (argv[0] == NULL)
		{
			free(line);
			free(argv);
			continue;
		}

		/* 2. Commandes built-in (exit, env) */
		if (check_builtin(argv, line, env, last_status))
		{
			free(line);
			free(argv);
			continue;
		}

		/* 3. Recherche du chemin dans le PATH */
		cmd_path = find_path(argv[0], env);

		/* 4. Si la commande n'existe pas : ERREUR 127 + PAS DE FORK */
		if (cmd_path == NULL)
		{
			print_error(av[0], argv[0], ": not found\n");
			free(line);
			free(argv);
			last_status = 127; /* <-- Code de retour 127 pour commande introuvable */
			continue;
		}

		/* 5. Exécution si la commande existe */
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
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
		}

		free(cmd_path);
		free(line);
		free(argv);
	}
	return (last_status);
}