#include "shell.h"

/**
 * get_path - Recupere la valeur de la variable PATH
 * @envp: Environnement systeme
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
 * @command: Commande tapee
 * @envp: Environnement systeme
 * Return: Pointeur alloue vers le chemin, ou NULL
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
