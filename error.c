#include "shell.h"
/**
 * print_error - Affiche un message d'erreur standardisé sur stderr
 * @shell_name: Nom de l'exécutable (av[0])
 * @command: Commande qui a échoué (argv[0])
 * @msg: Le message d'erreur spécifique (ex: ": not found\n")
 */
void print_error(char *shell_name, char *command, char *msg)
{
	write(STDERR_FILENO, shell_name, strlen(shell_name));
	write(STDERR_FILENO, ": 1: ", 5);
	write(STDERR_FILENO, command, strlen(command));
	write(STDERR_FILENO, msg, strlen(msg));
}
