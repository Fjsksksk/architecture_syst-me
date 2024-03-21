#include "planificateur.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void planificateur(int nb_repetition, int temps_attente, char* cmd){
    int i;
    for(i=0; i<nb_repetition; i++){
        printf("Execution de la commande %s\n", cmd);
        int duration = temps_execution(cmd);
        if(i != nb_repetition - 1){
            if(duration > temps_attente){
                sleep(temps_attente);
            }else{
                sleep(temps_attente - duration);
            }
        }else{
            printf("Fin de la planification\n");
        }
        
    }
}

int temps_execution(char* cmd){
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    system(cmd);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
    
}

int main(int argc, char* argv[]){
    //Demande nb de repetition
    int nb_repetition;
    printf("Entrez le nombre de répétition: ");
    scanf("%d", &nb_repetition);
    //Demande temps d'attente
    int temps_attente;
    printf("Entrez le temps d'attente: ");
    scanf("%d", &temps_attente);
    //Demande commande
    char cmd[100];
    printf("Entrez la commande: ");
    scanf("%s", cmd);



    planificateur(nb_repetition, temps_attente, cmd);
    return 0;
}