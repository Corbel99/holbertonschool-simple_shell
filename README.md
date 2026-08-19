# Simple Shell - `hsh`

## Description

`hsh` est un interpréteur de commandes UNIX minimaliste écrit en langage C dans le cadre du cursus Holberton School. Ce projet reproduit le fonctionnement de base d'un shell comme `sh` ou `bash`.

Le programme lit les lignes d'instructions saisies par l'utilisateur ou envoyées via un flux, découpe la commande en arguments, cherche l'exécutable correspondant dans les répertoires définis par la variable d'environnement `PATH`, puis lance son exécution au sein d'un processus enfant.

---

## Fonctionnalités

*   **Mode interactif** : Affiche le prompt `#cisfun$ ` lorsque le descripteur d'entrée standard est connecté à un terminal.
*   **Mode non-interactif** : Permet de transmettre des commandes via une redirection ou un tube de communication (ex: `echo "ls -l" | ./hsh`).
*   **Recherche dans le PATH** : Analyse de la variable d'environnement `PATH` pour exécuter les commandes sans devoir préciser leur chemin absolu.
*   **Commandes intégrées (built-ins)** :
    *   `exit` : Termine le processus du shell et renvoie le dernier statut d'exécution.
    *   `env` : Affiche l'ensemble des variables d'environnement courantes.
*   **Gestion des erreurs** : Retourne des messages d'erreur formatés sur la sortie d'erreur standard (`stderr`) et gère la fin de fichier (`Ctrl+D` / EOF).

---

## Structure des fichiers

*   **`shell.h`** : Fichier d'en-tête contenant la définition des structures, des macros, ainsi que tous les prototypes de fonctions.
*   **`main.c`** : Point d'entrée de l'application et boucle d'exécution principale.
*   **`input_handlers.c`** : Fonctions d'affichage du prompt, de lecture de la ligne d'entrée, d'analyse syntaxique (*parsing*) et de contrôle des *built-ins*.
*   **`path_utils.c`** : Logiciels d'extraction et de parcours de la variable `PATH` pour localiser les fichiers exécutables.
*   **`execute.c`** : Gestion de la création des processus enfants (`fork`), du remplacement d'image d'exécution (`execve`) et du suivi du statut du processus (`wait`).
*   **`error.c`** : Routine d'affichage des messages d'erreur sur le flux `stderr`.
*   **`man_1_simple_shell`** : Manuel de la commande formaté pour l'utilitaire `man`.

---

## Compilation

Le projet se compile avec `gcc` et les options d'avertissement et de norme recommandées :

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
$ ./hsh
#cisfun$ ls -la
total 48
drwxr-xr-x 2 user user 4096 Aug 19 14:30 .
drwxr-xr-x 4 user user 4096 Aug 19 14:00 ..
-rw-r--r-- 1 user user  284 Aug 19 14:00 error.c
-rw-r--r-- 1 user user  642 Aug 19 14:00 execute.c
-rw-r--r-- 1 user user 1894 Aug 19 14:00 input_handlers.c
-rw-r--r-- 1 user user  892 Aug 19 14:00 main.c
-rw-r--r-- 1 user user 1204 Aug 19 14:00 path_utils.c
-rw-r--r-- 1 user user  721 Aug 19 14:00 shell.h
-rwxr-xr-x 1 user user 17400 Aug 19 14:31 hsh
#cisfun$exit$
$ echo "pwd" | ./hsh
/home/user/holbertonschool-simple_shell
$
Projet développé par Thomas Corbel et Bilal Maksoud 
