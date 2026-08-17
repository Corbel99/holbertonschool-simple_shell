#include "shell.h"

/**
 * main - Shell simple exécutant des commandes avec leurs arguments.
 * @argc: Nombre d'arguments (non utilisé)
 * @argv: Tableau d'arguments (non utilisé)
 * @envp: Variables d'environnement
 *
 * Return: Toujours 0.
 */
int main(int argc, char **argv, char **envp)
{
	char *line;
	char **tokens;
	pid_t pid;
	int status;

	(void)argc;
	(void)argv;

	while (1)
	{
		display_prompt();

		line = read_line();

		tokens = parse_line(line);

		/* Si la ligne est vide (appui sur Entrée) */
		if (tokens[0] == NULL)
		{
			free(tokens);
			free(line);
			continue;
		}

		pid = fork();

		if (pid == -1)
		{
			perror("fork");
			free(tokens);
			free(line);
			continue;
		}

		/* --- PROCESSUS FILS --- */
		if (pid == 0)
		{
			if (execve(tokens[0], tokens, envp) == -1)
			{
				perror(tokens[0]);
				free(tokens);
				free(line);
				exit(1);
			}
		}

		/* --- PROCESSUS PARENT --- */
		if (pid > 0)
		{
			wait(&status);
			free(tokens);
		}

		/* Libération de la ligne saisie après chaque commande */
		free(line);
	}

	return (0);
}
