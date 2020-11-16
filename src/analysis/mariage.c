#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/aide.h"
#include "../../include/mariage.h"
#include "../../include/createIndividual.h"
#include "../../include/chaine.h"


// AFFICHAGE DES VALEURS DE LA LISTE MARIAGE
void display_values (wedding* ptr_tete) {
	wedding* ptr = ptr_tete;
	
	while (ptr != NULL) {
		
		printf("Value : %s\n",ptr->info_wedding.wedd.location);
		
		ptr = ptr->next;
	}
}


// CREATION D'UN MARIAGE
wedding *create_wedding (weddData value) {
	wedding* new = NULL;
	
	new = (wedding*)malloc(sizeof(wedding));
	new->info_wedding = value;
	new->next = NULL;
	
	return new;
}


// RECHERCHE DU DERNIER MARIAGE
wedding* find_last_wedding(wedding *ptr_tete) {
	wedding *ptr = ptr_tete;
	//wedding *tmp = NULL;
	
	while (ptr->next != NULL) {
		//tmp = ptr;
		ptr = ptr->next;
	}
	
	return ptr;
}


// INSERTION D'UN MARIAGE
void insert_wedding (wedding **ptr_tete, wedding *new) {
	
	if(*ptr_tete == NULL) {
		*ptr_tete = new;
	} else {
		new->next = *ptr_tete;
		*ptr_tete = new;
	}
}


// INSERTION D'UNE VALEUR DANS LA LISTE
void insert_value (wedding **ptr_tete, weddData value) {
	wedding* new = NULL;
	
	new = create_wedding(value);
	
	insert_wedding(ptr_tete,new);
}


// CREATION D'UNE LISTE SANS SAISIE DE VALEURS
/*wedding* create_list_wedd (weddData data) {
	wedding *ptrTete = NULL;
	wedding *mariage = NULL;
	wedding *prec = NULL;
	
	mariage = create_wedding(data);
	insert_wedding(&ptrTete,mariage,prec);
	
	return ptrTete;
}*/
void create_list_wedd (wedding **ptrTete, weddData data) {
	wedding *mariage = NULL;
	
	mariage = create_wedding(data);
	
	insert_wedding(ptrTete,mariage);
	
	//insert_value(ptrTete,data);
}


// COMPTEUR NOMBRE DE MARIAGE
int cpt_wedding(wedding *ptrTete) {
	wedding *ptr = ptrTete;
	int cpt = 0;
	
	while(ptr != NULL){
		
		ptr = ptr->next;
		
		cpt++;
	}
	
	return cpt;
}


// ECRITURE FICHIER MARIAGE
void ecris_fichier_mariage(char *nomSortie, wedding *ptrTete) {
	FILE* csv;
	csv = fopen(nomSortie, "wt");
	wedding *ptr = ptrTete;
	int nb;
	
	nb = cpt_wedding(ptrTete);
	
	//Ecriture de la 1ere ligne:
	fprintf(csv, "%d;nom_M;prenom_M;date_N_M;lieu_N_M;nG;nom_F;prenom_F;date_N_F;lieu_N_F;date_M;lieu_M\n",nb);
	
	
	while(ptr != NULL){
		//On ecris les donnees dans le fichier
		fprintf(csv,"%d;%s;%s;%d/%d/%d;%s;%d;%s;%s;%d/%d/%d;%s;%d/%d/%d;%s\n",
			ptr->info_wedding.husband->infos.num_gen,
			ptr->info_wedding.husband->infos.surname,
			ptr->info_wedding.husband->infos.name,
			ptr->info_wedding.husband->infos.birth.time.day,
			ptr->info_wedding.husband->infos.birth.time.month,
			ptr->info_wedding.husband->infos.birth.time.year,
			ptr->info_wedding.husband->infos.birth.location,
			ptr->info_wedding.wife->infos.num_gen,
			ptr->info_wedding.wife->infos.surname,
			ptr->info_wedding.wife->infos.name,
			ptr->info_wedding.wife->infos.birth.time.day,
			ptr->info_wedding.wife->infos.birth.time.month,
			ptr->info_wedding.wife->infos.birth.time.year,
			ptr->info_wedding.wife->infos.birth.location,
			ptr->info_wedding.wedd.time.day,
			ptr->info_wedding.wedd.time.month,
			ptr->info_wedding.wedd.time.year,
			ptr->info_wedding.wedd.location
			);
			printf("done");
		//Fin de traitement de l'individu
		ptr = ptr->next;
	}

	fclose(csv);
}


void import_wedding (char * nomfichier, individual **ptrtete, wedding **ptr_Tete) {
	FILE* csv;
	char **tab = NULL;
	char **time = NULL;
	char *ligne = NULL;
	int nb_sep = 0;
	int cpt = 0;
	
	ligne = (char*)malloc(sizeof(char)*500);
	
	csv = fopen(nomfichier, "rt");
	
	if(csv == NULL) { 
		printf("Le fichier n'existe pas! Fin.\n");
		return ;
	}
	
	individual *tHusband;
	individual *tWife;
	weddData donnees;
	indivData husband = initIndivData();
	indivData wife = initIndivData();
	
	printf("fichier identifié, lecture...\n");
	
	while(fgets(ligne,sizeof(char) * 500,csv) != NULL) {
		
		if (cpt >= 1) {
			
			// SEPARATION DES COLONNES
			fonct(ligne,&tab,&nb_sep,';');
			
			husband.num_gen = atoi(tab[0]);
			
			husband.surname = tab[1];
			
			husband.name = tab[2];
			
			nb_sep = 0;
			
			fonct(tab[3],&time,&nb_sep,'/');
			
			if(nb_sep != 0) {
				husband.birth.time.day = atoi(time[0]);
				
				husband.birth.time.month = atoi(time[1]);
				
				husband.birth.time.year = atoi(time[2]);
			} else {
				husband.birth.time.day = 0;
				
				husband.birth.time.month = 0;
				
				husband.birth.time.year = 0;
			}
			
			husband.birth.location = tab[4];
			
			wife.num_gen = atoi(tab[5]);
			
			wife.surname = tab[6];
			
			wife.name = tab[7];
			
			nb_sep = 0;
			
			fonct(tab[8],&time,&nb_sep,'/');
			
			if(nb_sep != 0) {
				wife.birth.time.day = atoi(time[0]);
				
				wife.birth.time.month = atoi(time[1]);
				
				wife.birth.time.year = atoi(time[2]);
			} else {
				wife.birth.time.day = 0;
				
				wife.birth.time.month = 0;
				
				wife.birth.time.year = 0;
			}
			
			wife.birth.location = tab[9];
			
			nb_sep = 0;
			
			fonct(tab[10],&time,&nb_sep,'/');
			
			if(nb_sep != 0) {
				donnees.wedd.time.day = atoi(time[0]);
				
				donnees.wedd.time.month = atoi(time[1]);
				
				donnees.wedd.time.year = atoi(time[2]);
			} else {
				donnees.wedd.time.day = 0;
				
				donnees.wedd.time.month = 0;
				
				donnees.wedd.time.year = 0;
			}
			
			donnees.wedd.location = tab[11];
			
			if(rechercheIndiv(husband.surname,husband.name,husband.num_gen,*ptrtete) != NULL) {
				donnees.husband = rechercheIndiv(husband.surname,husband.name,husband.num_gen,*ptrtete);
			} else {
				tHusband = creerIndiv(husband,ptrtete,NULL,NULL);
				donnees.husband = tHusband;
				insereIndiv(donnees.husband,ptrtete,NULL);
			}
			
			if(rechercheIndiv(wife.surname,wife.name,wife.num_gen,*ptrtete) != NULL) {
				donnees.wife = rechercheIndiv(wife.surname,wife.name,wife.num_gen,*ptrtete);
			} else {
				tWife = creerIndiv(wife,ptrtete,NULL,NULL);
				donnees.wife = tWife;
				insereIndiv(donnees.wife,ptrtete,NULL);
			}
			
			create_list_wedd(ptr_Tete,donnees);
		}
		cpt++;
	}
	
	fclose(csv);
            
    printf("\nLe fichier a permis le traitement de %d mariages \n",cpt-1);
}

