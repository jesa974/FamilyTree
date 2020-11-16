#include <stdio.h>
#include <stdlib.h>
#include "../../include/aide.h"
#include "../../include/weddLink.h"

void createWeddLinkHusband(wedding *ptr) {
    wedding *try = ptr;
    weddLink *nouv = NULL;
    
    while(try != NULL) { //If wedding list is existing
        
        if(try->info_wedding.husband != NULL) { //If husband is existing
			
            if(try->info_wedding.husband->firstUnion == NULL) { //If union list doesn't exist
				nouv = (weddLink*)malloc(sizeof(weddLink));
				nouv->mariage = try;
                nouv->next = NULL;
                try->info_wedding.husband->firstUnion = nouv;
                nouv = NULL ;
            } else {
                nouv =(weddLink*)malloc(sizeof(weddLink));
                nouv->mariage = try;
                nouv->next = try->info_wedding.husband->firstUnion;
				try->info_wedding.husband->firstUnion = nouv ;
				nouv = NULL ;
            }
        }
        try = try->next;
    }
}


void createWeddLinkWife(wedding *ptr) {
    wedding *try = ptr;
    weddLink *nouv = NULL;
        
    while(try != NULL) { //If wedding list is existing
        
        if(try->info_wedding.wife != NULL) { //If wife is existing
			
            if(try->info_wedding.wife->firstUnion == NULL) { //If union list doesn't exist
				nouv = (weddLink*)malloc(sizeof(weddLink));
				nouv->mariage = try;
                nouv->next = NULL;
                try->info_wedding.wife->firstUnion = nouv;
                nouv = NULL ;
            } else {
                nouv =(weddLink*)malloc(sizeof(weddLink));
                nouv->mariage = try;
                nouv->next = try->info_wedding.wife->firstUnion;
				try->info_wedding.wife->firstUnion = nouv ;
				nouv = NULL ;
            }
        }
        try = try->next;
    }
}


void afficheWeddLink(individual *ptrTete) {
    individual *PTR = ptrTete;
    weddLink *l_ptr = NULL;
        
    while(PTR != NULL){
	
		if(PTR->firstUnion != NULL) {
			printf("    Marié : %s %s %hd/%hd/%hd\n",PTR->infos.surname,PTR->infos.name,PTR->infos.birth.time.day,PTR->infos.birth.time.month,PTR->infos.birth.time.year);
			l_ptr = PTR->firstUnion;
			
			while(l_ptr != NULL) {
				if(l_ptr->mariage->info_wedding.wedd.time.day != 0) {
					printf("         |-> Mariage : %s %s %s %hd/%hd/%hd\n",l_ptr->mariage->info_wedding.wife->infos.surname,l_ptr->mariage->info_wedding.wife->infos.name,l_ptr->mariage->info_wedding.wedd.location,l_ptr->mariage->info_wedding.wedd.time.day,l_ptr->mariage->info_wedding.wedd.time.month,l_ptr->mariage->info_wedding.wedd.time.year);
				} else {
					printf("         |-> Mariage : %s %s %s\n",l_ptr->mariage->info_wedding.wife->infos.surname,l_ptr->mariage->info_wedding.wife->infos.name,l_ptr->mariage->info_wedding.wedd.location);
				}
				l_ptr = l_ptr->next;
			}
		} else {
			printf("NON Marié : %s %s\n",PTR->infos.surname,PTR->infos.name);
		}
		
		PTR = PTR->next;
	}
}
