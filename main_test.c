#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_TOKENS 64

/**
 * main - Shell simple exécutant des commandes avec leurs arguments.
 *
 * Return: Toujours 0.
 */
int main(int argc, char **argv, char **envp)
{
	char *line = NULL;      /* Pointeur pour stocker la ligne lue par getline */
	size_t len = 0;         /* Taille du tampon alloué dynamiquement par getline */
	pid_t pid;              /* PID pour distinguer le processus parent du fils */
	int status;             /* Statut de sortie du processus fils récupéré par wait */
	int i;                  /* Indice pour remplir le tableau d'arguments */
	char **exec_argv;       /* Tableau d'arguments requis par execve */
	char *token;            /* Pointeur vers chaque mot extrait par strtok */

	(void)argc;
	(void)argv;

	while (1)
	{
		/* Affiche le prompt uniquement si l'entrée est un terminal interactif */
		if (isatty(STDIN_FILENO))
			printf("#cisfun$ ");

		/* Lit la ligne saisie. Retourne -1 en cas de fin de fichier (Ctrl+D) ou d'erreur */
		if (getline(&line, &len, stdin) == -1)
			break;

		/* Allocation du tableau de pointeurs pour stocker la commande et ses arguments */
		exec_argv = malloc(sizeof(char *) * MAX_TOKENS);
		if (exec_argv == NULL)
		{
			perror("malloc");
			free(line);
			return (1);
		}

		/* Découpage de la ligne selon les espaces, tabulations et retours à la ligne */
		i = 0;
		token = strtok(line, " \t\n");
		while (token != NULL && i < MAX_TOKENS - 1)
		{
			exec_argv[i] = token;
			i++;
			token = strtok(NULL, " \t\n");
		}
		exec_argv[i] = NULL; /* execve exige que le tableau se termine par NULL */

		/* Si la ligne était vide (ex: simple appui sur Entrée), on passe au tour suivant */
		if (exec_argv[0] == NULL)
		{
			free(exec_argv);
			continue;
		}

		/* Duplication du processus */
		pid = fork();

		if (pid == -1)
		{
			perror("fork");
			free(exec_argv);
			free(line);
			return (1);
		}

		/* --- PROCESSUS FILS --- */
		if (pid == 0)
		{
			/* Remplace l'image du processus par le nouveau programme */
			execve(exec_argv[0], exec_argv, envp);

			/* Le code ci-dessous ne s'exécute QUE si execve échoue */
			perror("execve");
			free(exec_argv);
			free(line);
			exit(1);
		}

		/* --- PROCESSUS PARENT --- */
		if (pid > 0)
		{
			/* Attend la fin d'exécution du processus fils */
			wait(&status);
			
			/* Libère le tableau d'arguments avant de relancer la boucle */
			free(exec_argv);
		}
	}

	/* Libération de la mémoire globale allouée par getline à la sortie du shell */
	free(line);
	return (0);
}
