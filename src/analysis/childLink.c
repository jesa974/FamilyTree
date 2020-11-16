#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/aide.h"
#include "../../include/createIndividual.h"

childLink* creerChildLink(individual *PTR){
    childLink *fils = NULL;
    fils = (childLink*)malloc(sizeof(childLink));
   
    fils->child = PTR;
    fils->next = NULL;

	return fils;
}


childLink* trieChildLink(childLink *ptrTete, individual *PTR){
    //childLink *PTRfils = NULL;
    childLink *fils = NULL;
    //childLink *precedent = NULL;

    /*fils = malloc(sizeof(childLink));
    
    fils->child = PTR;
    fils->next = NULL;

    //TODO: precedent = chercheDatePrec(); pour plus tard
	ptrTete->next = fils;
    PTRfils = ptrTete;*/
    
    fils = malloc(sizeof(childLink));
    
    fils->child = PTR;
    fils->next = ptrTete;

    //TODO: precedent = chercheDatePrec(); pour plus tard
	ptrTete = fils;
    
	//return PTRfils;
	return ptrTete;
}


void afficheChildLink(individual *ptrTete) {
    individual *PTR = ptrTete;
    childLink *l_ptr = NULL;
        
    while(PTR != NULL){
	
		if(PTR->firstChild != NULL){
			printf("    Parent : %s %s\n",PTR->infos.surname,PTR->infos.name);
			l_ptr = PTR->firstChild;
			
			while(l_ptr != NULL) {
				printf("         |-> Enfant : %s %s\n",l_ptr->child->infos.surname,l_ptr->child->infos.name);
				l_ptr = l_ptr->next;
			}
		} else {
			printf("NON Parent : %s %s\n",PTR->infos.surname,PTR->infos.name);
		}
		
		PTR = PTR->next;
	}
}


void childLinkerFath(individual *ptrTete){
    individual *var = ptrTete;
    childLink *nouv = NULL;
        
    while(var != NULL) { //If individual list is existing
        
        if(var->father != NULL) { //If father is existing
			
            if(var->father->firstChild == NULL) { //If child list doesn't exist
				
				nouv =(childLink*)malloc(sizeof(childLink));
				nouv->child = var;
                nouv->next = NULL;
                var->father->firstChild = nouv;
                nouv = NULL ;
            } else {
				
                nouv =(childLink*)malloc(sizeof(childLink));
                nouv->child = var;
                nouv->next = var->father->firstChild;
				var->father->firstChild = nouv ;
				nouv = NULL ;
            }
        }
        var = var->next;
    }
}


void childLinkerMoth(individual *ptrTete){
    individual *var = ptrTete;
    childLink *nouv = NULL;
        
    while(var != NULL) { //If individual list is existing
        
        if(var->mother != NULL) { //If father is existing
			
            if(var->mother->firstChild == NULL) { //If child list doesn't exist
				
				nouv =(childLink*)malloc(sizeof(childLink));
				nouv->child = var;
                nouv->next = NULL;
                var->mother->firstChild = nouv;
                nouv = NULL ;
            } else {
				
                nouv =(childLink*)malloc(sizeof(childLink));
                nouv->child = var;
                nouv->next = var->mother->firstChild;
				var->mother->firstChild = nouv ;
				nouv = NULL ;
            }
        }
        var = var->next;
    }
}
