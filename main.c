#include "shell.h"

/**
 * main - Boucle principale du simple shell
 * @ac: Nombre d'arguments (non utilisé)
 * @av: Tableau d'arguments du shell
 * @env: Environnement système
 * Return: Dernier code de statut d'exécution
 */
int main(int ac, char **av, char **env)
{
	char *line, **argv, *cmd_path;
	int last_status = 0;

	(void)ac;
	while (1)
	{
		display_prompt();
		line = read_line(&last_status);
		argv = parse_line(line);

		if (argv[0] == NULL || check_builtin(argv, line, env, last_status))
		{
			free(line);
			free(argv);
			continue;
		}

		cmd_path = find_path(argv[0], env);
		if (cmd_path == NULL)
		{
			print_error(av[0], argv[0], ": not found\n");
			last_status = 127;
		}
		else
		{
			last_status = execute_command(cmd_path, argv, env);
			free(cmd_path);
		}
		free(line);
		free(argv);
	}
	return (last_status);
}
