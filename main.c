#include "shell.h"

/**
 * main - Boucle principale du simple shell
 * @ac: Nombre d'arguments
 * @av: Tableau d'arguments
 * @env: Environnement système
 * Return: 0 en cas de succès
 */
int main(int ac, char **av, char **env)
{
	char *line;
	char **argv;
	(void)ac;
	(void)av;

	while (1)
	{
		/* 1. Appel affichage du prompt */
		display_prompt();

		/* 2. Appel lecture de la ligne */
		line = read_line();

		/* 3. Appel découpage de la ligne */
		argv = parse_line(line);

		/* Gestion ligne vide */
		if (argv[0] == NULL)
		{
			free(line);
			free(argv);
			continue;
		}

		/* 4. Appel vérification des built-ins */
		if (check_builtin(argv, line, env))
		{
			free(line);
			free(argv);
			continue;
		}

		/* Part de ton binôme : find_path(), fork(), execve() */

		free(line);
		free(argv);
	}
	return (0);
}
