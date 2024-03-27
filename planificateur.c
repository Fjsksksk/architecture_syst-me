// test :./planificateur 2 5 "ls" ou ./planificateur 2 5 "echo 'bjr'; sleep 10; echo 'bye';"

#include "planificateur.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

void executer_commande(const char *commande)
{
    // Lancement de la commande via le shell
    execl("/bin/sh", "sh", "-c", commande, NULL);

    // Si execl échoue, affiche une erreur
    perror("Erreur lors de l'exécution de la commande");
    exit(EXIT_FAILURE);
}

void planifier_taches(const char *commande, int delai, int iterations)
{
    // Boucle sur le nombre d'itérations
    for (int i = 0; i < iterations; i++)
    {
        // Lancement d'un processus fils pour exécuter la commande
        pid_t pid = fork();

        if (pid == -1)
        {
            // Erreur lors de la création du processus fils
            perror("Erreur lors de la création du processus fils");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Processus fils : exécute la commande
            executer_commande(commande);
            exit(EXIT_SUCCESS); // Quitte le processus fils après l'exécution de la commande
        }
        else
        {
            // Processus parent : attend le délai entre chaque itération
            if (i < iterations - 1) // Attend seulement si ce n'est pas la dernière itération
                sleep(delai);
        }
    }

    // Attend la fin de toutes les itérations avant de terminer le processus parent
    for (int i = 0; i < iterations; i++)
    {
        wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    // Vérifie le nombre d'arguments
    if (argc != 4)
    {
        fprintf(stderr, "Utilisation du planificateur: %s <iterations> <delai> <\"commande\">\n", argv[0]);
        return 1;
    }

    // Récupère les arguments de la ligne de commande
    int iterations = atoi(argv[1]);
    int delai = atoi(argv[2]);
    const char *commande = argv[3];

    // Vérifie que le délai est supérieur à zéro
    if (delai <= 0)
    {
        fprintf(stderr, "Erreur : Le délai doit être supérieur à zéro.\n");
        return 1;
    }
    // Vérifie qu'il y a au moins une itération
    if (iterations <= 0)
    {
        fprintf(stderr, "Erreur : Le nombre d'itérations doit être supérieur à 0.\n");
        return 1;
    }

    // Appel de la fonction de planification des tâches
    planifier_taches(commande, delai, iterations);

    return 0;
}
