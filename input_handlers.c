#include "shell.h"

/**
 * display_prompt - Affiche le prompt si le terminal est en mode interactif
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "#cisfun$ ", 9);
}

/**
 * read_line - Lit une ligne depuis l'entrée standard
 * Return: Pointeur vers la chaîne lue
 */
char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t read_bytes;

	read_bytes = getline(&line, &bufsize, stdin);
	if (read_bytes == -1)
	{
		free(line);
		if (feof(stdin))
			exit(EXIT_SUCCESS);
		perror("Error readline");
		exit(EXIT_FAILURE);
	}
	return (line);
}

/**
 * parse_line - Découpe une ligne en tableau de mots
 * @line: La ligne à découper
 * Return: Tableau de pointeurs (argv)
 */
char **parse_line(char *line)
{
	int bufsize = 64, i = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		perror("Allocation error");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, DELIM);
	while (token != NULL)
	{
		tokens[i] = token;
		i++;

		if (i >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				perror("Reallocation error");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, DELIM);
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * check_builtin - Vérifie et exécute les built-ins
 * @argv: Tableau d'arguments
 * @line: Buffer de la ligne
 * @env: Variable d'environnement
 * Return: 1 si built-in exécuté, 0 sinon
 */
int check_builtin(char **argv, char *line, char **env)
{
	int i = 0;

	if (argv[0] == NULL)
		return (0);

	if (strcmp(argv[0], "exit") == 0)
	{
		free(line);
		free(argv);
		exit(EXIT_SUCCESS);
	}

	if (strcmp(argv[0], "env") == 0)
	{
		while (env && env[i])
		{
			write(STDOUT_FILENO, env[i], strlen(env[i]));
			write(STDOUT_FILENO, "\n", 1);
			i++;
		}
		return (1);
	}

	return (0);
}
