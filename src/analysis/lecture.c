#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/aide.h"


void afficheListe(individual *ptrTeteIndividu){
    individual *ptr = ptrTeteIndividu;
    int i=0;

    printf("\nLecture des individus...\n\n");

    while(ptr != NULL){
        i++;
        printf("Individu %d : %s %s\n", i, ptr->infos.name, ptr->infos.surname);
        printf("Génération : %d\n", ptr->infos.num_gen);
        printf("Genre : %c\n", ptr->infos.gender);
        printf("Né le : %d/%d/%d\n", ptr->infos.birth.time.day, ptr->infos.birth.time.month, ptr->infos.birth.time.year);
        printf("Lieu : %s\n", ptr->infos.birth.location);
        if(ptr->infos.death.time.day != 0){
            printf("Mort le : %d/%d/%d\n", ptr->infos.death.time.day, ptr->infos.death.time.month, ptr->infos.death.time.year);
            printf("Lieu : %s\n", ptr->infos.death.location);
        }
        else
            printf("Mort : Pas encore\n");
        if(ptr->father != NULL){
             printf("Père : %s %s\n", ptr->father->infos.name, ptr->father->infos.surname );
        }
        if(ptr->mother != NULL){
             printf("Mère : %s %s\n", ptr->mother->infos.name, ptr->mother->infos.surname );
        }

        printf("\n\n");
        ptr = ptr->next;
    }
}
