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
		exit(EXIT_SUCCESS);
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

/**
 * get_path - Récupère la valeur de la variable PATH
 * @envp: Environnement système
 * Return: Pointeur vers la valeur du PATH, ou NULL si absent ou vide
 */
char *get_path(char **envp)
{
	int i;

	if (!envp)
		return (NULL);

	for (i = 0; envp[i] != NULL; i++)
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			if (envp[i][5] == '\0')
				return (NULL);
			return (envp[i] + 5);
		}
	}
	return (NULL);
}

/**
 * find_path - Trouve le chemin complet d'une commande
 * @command: Commande tapée
 * @envp: Environnement système
 * Return: Pointeur alloué vers le chemin, ou NULL
 */
char *find_path(char *command, char **envp)
{
	char *path, *path_copy, *token, *pathname;

	if (!command || !*command)
		return (NULL);
	if (strchr(command, '/') != NULL)
		return (access(command, X_OK) == 0 ? strdup(command) : NULL);

	path = get_path(envp);
	if (!path)
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	token = strtok(path_copy, ":");
	while (token)
	{
		pathname = malloc(strlen(token) + strlen(command) + 2);
		if (!pathname)
			break;
		strcpy(pathname, token);
		strcat(pathname, "/");
		strcat(pathname, command);
		if (access(pathname, X_OK) == 0)
		{
			free(path_copy);
			return (pathname);
		}
		free(pathname);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
