// test :
// normal = ./planificateur 2 5 "ls"
// chevauchement de 2 commandes = ./planificateur 2 5 "echo 'bjr'; sleep 10; echo 'bye';"
// infini =  ./planificateur i 5 "ls"
// date = ./planificateur 2 5 "ls" 11/04/2024-09:00
// date + infini + chevauchement = ./planificateur i 3 "echo 'bjr'; sleep 10; echo 'bye';" 11/04/2024-09:00

#include "planificateur.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

// Fonction pour exécuter une commande via le shell
void executer_commande(const char *commande)
{
    printf("commande exécuté\n");
    // Lancement de la commande via le shell
    execl("/bin/sh", "sh", "-c", commande, NULL);

    // Si execl échoue, affiche une erreur
    perror("Erreur lors de l'exécution de la commande");
    exit(EXIT_FAILURE);
}

// Fonction pour planifier les tâches en fonction du délai et du nombre d'itérations
void planifier_taches(const char *commande, int delai, int iterations)
{
    // Si le nombre d'itérations est infini, défini iterations sur -1
    if (iterations == -1)
    {
        // Boucle infinie
        while (1)
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
                sleep(delai);
            }
        }
    }
    else
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
    }
    // Attend la fin de toutes les itérations avant de terminer le processus parent
}

// Fonction pour créer un processus fils et planifier les tâches
void fork_planifier(const char *commande, int delai, int iterations)
{
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
        planifier_taches(commande, delai, iterations);
        exit(EXIT_SUCCESS); // Quitte le processus fils après l'exécution de la commande
    }
}

// Fonction pour récuperer les informations donner par l'uttlisateur et lancer le planificateur en fonction d'elle.
int main(int argc, char *argv[])
{
    // Vérifie le nombre d'arguments
    if (argc != 4 && argc != 5)
    {
        fprintf(stderr, "\nUtilisation du planificateur: %s <iterations> <delai> <\"commande\"> <date (facultatif)>\n\nOptions :\ninfini : <itération> = i \ndate : <date> = JJ/MM/AAAA-HH:MM\n\nStop : Ctrl+C\n", argv[0]);
        return 1;
    }

    // Récupère les arguments de la ligne de commande
    int iterations;
    if (strcmp(argv[1], "i") == 0)
    {
        iterations = -1; // Infini
    }
    else
    {
        iterations = atoi(argv[1]);
    }
    int delai = atoi(argv[2]);
    const char *commande = argv[3];

    // Vérifie que le délai est supérieur à zéro
    if (delai <= 0)
    {
        fprintf(stderr, "Erreur : Le délai doit être supérieur à zéro.\n");
        return 1;
    }
    // Vérifie qu'il y a au moins une itération ou different d'infini
    if (iterations != -1 && iterations <= 0)
    {
        fprintf(stderr, "Erreur : Le nombre d'itérations doit être supérieur à 0.\n");
        return 1;
    }
    if (argc == 5)
    {
        // Vérifie si la date est au bon format
        char *date = argv[4];
        if (strlen(date) != 16 || date[2] != '/' || date[5] != '/' || date[10] != '-' || date[13] != ':')
        {
            fprintf(stderr, "Erreur : Format de date incorrect. Utilisez JJ/MM/AAAA-HH:MM.\n");
            return 1;
        }

        // Obtention de la date actuelle
        time_t date_actuelle;
        struct tm *tm_date_actuelle;
        time(&date_actuelle);
        tm_date_actuelle = localtime(&date_actuelle);

        // Conversion de la date uttlisateur en struct tm
        struct tm tm_date_utilisateur;
        sscanf(date, "%d/%d/%d-%d:%d", &tm_date_utilisateur.tm_mday, &tm_date_utilisateur.tm_mon, &tm_date_utilisateur.tm_year, &tm_date_utilisateur.tm_hour, &tm_date_utilisateur.tm_min);
        tm_date_utilisateur.tm_mon -= 1;     // Mois de 0 à 11
        tm_date_utilisateur.tm_year -= 1900; // Année - 1900

        tm_date_utilisateur.tm_hour -= 1; // Décrémente l'heure de 1, Je ne sais pas pourquoi mais lors du debug l'heure spécifiée prenais 1 heure XD
        time_t date_utilisateur = mktime(&tm_date_utilisateur);
        int difference = difftime(date_utilisateur, date_actuelle);

        // Si la différence est négative, la date spécifiée est antérieure à la date actuelle
        if (difference < 0)
        {
            printf("Date spécifiée antérieure à la date actuelle. Exécution immédiate de la commande.\n");
            fork_planifier(commande, delai, iterations);
            exit(EXIT_SUCCESS);
        }
        else if (difference == 0)
        {
            printf("Date spécifiée identique à la date actuelle. Exécution immédiate de la commande.\n");
            fork_planifier(commande, delai, iterations);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Attente jusqu'à la date spécifiée
            printf("Attente jusqu'à la date spécifiée. Temps d'attente : %d secondes\n", difference);
            sleep(difference);

            // Exécution de la commande après l'attente
            fork_planifier(commande, delai, iterations);
        }
    }
    else // Si aucune date spécifiée, exécute immédiatement la commande
    {
        fork_planifier(commande, delai, iterations);
    }

    return 0;
}