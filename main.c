#include "shell.h"

/**
 * main - Boucle principale du simple shell
 * @ac: Nombre d'arguments (non utilisé)
 * @av: Tableau d'arguments (non utilisé)
 * @env: Environnement système
 * Return: 0 en cas de succès
 */
int main(int ac, char **av, char **env)
{
    char *line;
    char **argv;
    pid_t pid;
    int status;

    (void)ac;
    (void)av;

    while (1)
    {
        display_prompt();
        line = read_line();
        argv = parse_line(line);

        /* Gestion ligne vide */
        if (argv[0] == NULL)
        {
            free(line);
            free(argv);
            continue;
        }

        /* Gestion des built-ins */
        if (check_builtin(argv, line, env))
        {
            free(line);
            free(argv);
            continue;
        }

        /* Exécution directe avec argv[0] */
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            free(line);
            free(argv);
            continue;
        }

        if (pid == 0)
        {
            if (execve(argv[0], argv, env) == -1)
            {
                perror(argv[0]);
                free(line);
                free(argv);
                exit(1);
            }
        }
        else
        {
            wait(&status);
        }

        free(line);
        free(argv);
    }
    return (0);
}
