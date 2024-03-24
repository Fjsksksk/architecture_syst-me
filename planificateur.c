#include "planificateur.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

void planifier_taches(const char *commande, int delai, int iterations)
{
    // vérification du délai
    if (delai <= 0)
    {
        fprintf(stderr, "Erreur : Le délai doit être supérieur à zéro.\n");
        return;
    }

    // Boucle sur le nombre d'itérations
    for (int i = 0; i < iterations; i++)
    {
        // Exécution de la commande
        printf("Exécution de la commande : %s\n", commande);
        system(commande);

        // Calcul du temps d'attente avant la prochaine exécution
        if (i != iterations - 1) // Si ce n'est pas la dernière itération
        {
            // Enregistrement du temps de départ
            time_t start_time;
            time(&start_time);

            // Calcul du temps d'attente
            int remaining_time = delai - (time(NULL) - start_time) % delai;
            printf("Attente jusqu'à la prochaine exécution : %d secondes\n", remaining_time);
            sleep(remaining_time);
        }
        else
        {
            printf("Fin des itérations.\n");
        }
    }
}

int main(int argc, char *argv[])
{
    // Vérification du nombre d'arguments
    if (argc != 4)
    {
        fprintf(stderr, "Fonctionnement: %s <\"commande\"> <delai> <iterations>\n", argv[0]);
        return 1;
    }

    // Récupération des arguments de la ligne de commande
    const char *commande = strtok(argv[1], "\"");
    int delai = atoi(argv[2]);
    int iterations = atoi(argv[3]);

    // Vérification que le délai est supérieur à zéro
    if (delai <= 0)
    {
        fprintf(stderr, "Erreur : Le délai doit être supérieur à zéro.\n");
        return 1;
    }
    // Vérification qu'il y ai minimum une itération
    if (delai <= 0)
    {
        fprintf(stderr, "Erreur : Le nombre d'itérations doit être supérieur à 1.\n");
        return 1;
    }

    // Appel de la fonction de planification des tâches
    planifier_taches(commande, delai, iterations);

    return 0;
}
