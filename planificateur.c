#include "planificateur.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h> // Pour utiliser strtok

void planifier_taches(const char *commande, int delai, int iterations)
{
    if (delai <= 0)
    {
        fprintf(stderr, "Erreur : Le délai doit être supérieur à zéro.\n");
        return;
    }

    time_t start_time;
    time(&start_time); // Enregistrement du temps de départ

    for (int i = 0; i < iterations; i++)
    {
        // Exécution de la commande
        printf("Exécution de la commande : %s\n", commande);
        system(commande);

        // Calcul du temps écoulé depuis le début de l'exécution
        time_t current_time;
        time(&current_time);
        double elapsed_time = difftime(current_time, start_time);

        // Calcul du temps d'attente jusqu'à la prochaine exécution
        int remaining_time = delai - ((int)elapsed_time % delai);
        printf("Attente jusqu'à la prochaine exécution : %d secondes\n", remaining_time);
        sleep(remaining_time);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <\"commande\"> <delai> <iterations>\n", argv[0]);
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

    // Appel de la fonction de planification des tâches
    planifier_taches(commande, delai, iterations);

    return 0;
}
