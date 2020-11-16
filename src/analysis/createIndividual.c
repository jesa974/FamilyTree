#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/aide.h"
#include "../../include/createIndividual.h"
#include "../../include/chaine.h"
#include "../../include/childLink.h"


int verifDate(date ref, date test) {
	
	int verif = 0;
	
	if(ref.year == test.year) {
		
		if(ref.month == test.month) {
			
			if(ref.day == test.day) {
				
				verif = 0;
				
			} else {
				
				verif = -1;
				
			}
			
		} else {
			
			verif = -1;
			
		}
		
	} else {
		
		verif = -1;
		
	}
	
	return verif;
}


int dateNulle(date ref) {
	
	int verif = 0;
	
	if(ref.year == 0) {
		
		if(ref.month == 0) {
			
			if(ref.day == 0) {
				
				verif = 0;
				
			} else {
				
				verif = -1;
				
			}
			
		} else {
			
			verif = -1;
			
		}
		
	} else {
		
		verif = -1;
		
	}
	
	return verif;
}


indivData initIndivData(void) {
	indivData donnees;
	
	donnees.num_gen = 0;
	donnees.name = "";
	donnees.surname = "";
	donnees.gender = ' ';
	
	donnees.birth.location = "";
	donnees.birth.time.day = 0;
	donnees.birth.time.month = 0;
	donnees.birth.time.month = 0;
	
	donnees.death.location = "";
	donnees.death.time.day = 0;
	donnees.death.time.month = 0;
	donnees.death.time.year = 0;
	
	return donnees;
}


individual* cherchePrecedent(individual indiv,individual *ptrTete){
	individual *ptr = ptrTete;
	individual *precedent = NULL;
	while(ptr != NULL){
		
		if(ptr->infos.num_gen >= indiv.infos.num_gen) {
			
			if(ordreAlpha(ptr->infos.surname,indiv.infos.surname) == 2) {
				
				if(ordreAlpha(ptr->infos.name,indiv.infos.name) == 2) {
					precedent = ptr;
					ptr = ptr->next;
				} else {
					ptr = ptr->next;
				}
				
			} else {
				ptr = ptr->next;
			}
			
		} else {
			ptr = ptr->next;
		}
	}
	
	return precedent;
} 

void insereIndiv(individual *indiv, individual **ptrTete, individual *precedent){
	
	individual *tmp = NULL;
	
	tmp = cherchePrecedent(*indiv,*ptrTete);
	
	if(tmp == NULL){
		indiv->next = *ptrTete;
		*ptrTete = indiv;
	}else{
		indiv->next = tmp->next;
		tmp->next = indiv;
	}
	
	precedent = tmp;
}

individual* rechercheIndiv(char *nom,char *prenom, int gen, individual *ptrTete){
	individual *indiv = NULL;
	individual *ptr = ptrTete;
	
	while(ptr != NULL){
		
		if(strcmp(ptr->infos.name,prenom) == 0 && strcmp(ptr->infos.surname,nom) == 0 && ptr->infos.num_gen == gen){
			indiv = ptr;
			return indiv;
		}
		ptr = ptr->next;
	}
	
	return indiv;
}


int compareIndiv(individual *src, individual *other){
	
	if(src != NULL && other != NULL) {
		
		if(strcmp(src->infos.name,other->infos.name) == 0 && strcmp(src->infos.surname,other->infos.surname) == 0 && src->infos.num_gen == other->infos.num_gen){
			return 0;
		} else {
			return 1;
		}
	} else {
		return 2;
	}
}


individual* creerIndiv(indivData data, individual **ptrTete,individual *pere, individual *mere){
	individual *indiv;
	
	indiv = (individual*)malloc(sizeof(individual));
	indiv->infos = data;
	
	indiv->mother = mere;
	
	indiv->father = pere;
	
	indiv->firstChild = NULL;
	indiv->firstUnion = NULL;
	
	return indiv;
}


void newIndividual(indivData data, individual **ptrTete,individual *pere, individual *mere){
	individual *indiv;
	individual *precedent = NULL;
	
	indiv = creerIndiv(data,ptrTete,pere,mere);
	
	insereIndiv(indiv,ptrTete,precedent);
}

void supprIndiv(individual *ptrSupr,individual **ptrTete){
	individual *precedent;
	
	precedent = cherchePrecedent(*ptrSupr,*ptrTete);
	
	if(precedent == NULL) {
		*ptrTete = ptrSupr->next;
		ptrSupr->next = NULL;
	} else {
		precedent->next = ptrSupr->next;
		ptrSupr->next = NULL;
	}
}

void afficheIndiv(individual *ptrTete){
		individual *ptr;
		//childLink *var;
		int cpt = 0;
		ptr = ptrTete;
		
		//printf("\n%p",&ptrTete) ;
		
		while(ptr != NULL){
			printf("\nnum : %d\n",ptr->infos.num_gen);
			printf("nom : %s\n",ptr->infos.surname);
			printf("prenom : %s\n",ptr->infos.name);
			
			if(ptr->infos.birth.time.day != 0) {
				printf("date naissance : %d/%d/%d\n",ptr->infos.birth.time.day,ptr->infos.birth.time.month,ptr->infos.birth.time.year);
			}
			
			if(strcmp(ptr->infos.birth.location,"") != 0) {
				printf("lieu naissance : %s\n",ptr->infos.birth.location);
			}
			
			if(ptr->infos.death.time.day != 0) {
				printf("date mort : %d/%d/%d\n",ptr->infos.death.time.day,ptr->infos.death.time.month,ptr->infos.death.time.year);
			}
			
			if(strcmp(ptr->infos.death.location,"") != 0) {
				printf("lieu mort : %s\n",ptr->infos.death.location);
			}
			
			if(ptr->father != NULL) {
				printf("prenom papa : %s\n",ptr->father->infos.name);
			}
			
			if(ptr->mother != NULL) {
				printf("prenom maman : %s\n",ptr->mother->infos.name);
			}
			
			afficheChildLink(ptr);
			
			//printf("%p\n",ptr);
			ptr = ptr->next;
			cpt++;
		}
		printf("%d\n",cpt);
}


int compteIndiv(individual *ptrTete) {
	individual *ptr = ptrTete;
	int total = 0;
	
	while(ptr != NULL) {
		total++;
		ptr = ptr->next;
	}
	
	return total;
}


void import_indiv (char * nomfichier, individual **ptrtete) {
	FILE* csv;
	char **tab = NULL;
	char **time = NULL;
	char *ligne = NULL;
	int nb_sep = 0;
	int cpt = 0;
	
	//int nblig = 0;
	
	ligne = (char*)malloc(sizeof(char)*500);
	
	char *cmd = "dos2unix -o ";
	
	concatChaine(&cmd,nomfichier);
	
	system(cmd);
	
	csv = fopen(nomfichier, "rt");
	
	if(csv == NULL) { 
		printf("Le fichier n'existe pas! Fin.\n");
		return ;
	}
	
	indivData *pers;
	indivData *father;
	indivData *mother;
	
	pers = (indivData*)malloc(sizeof(indivData)*cpt_ligne(nomfichier));
	father = (indivData*)malloc(sizeof(indivData)*cpt_ligne(nomfichier));
	mother = (indivData*)malloc(sizeof(indivData)*cpt_ligne(nomfichier));
	
	printf("fichier identifié, lecture...\n");
	
	while(fgets(ligne,sizeof(char) * 500,csv) != NULL) {
		
		//nblig++;
		//printf("Ligne numero : %d\n",nblig);
		
		if (cpt >= 1) {
			
			// SEPARATION DES COLONNES
			fonct(ligne,&tab,&nb_sep,';');
			
			pers[cpt-1].num_gen = atoi(tab[0]);
			
			pers[cpt-1].surname = tab[1];
			
			pers[cpt-1].name = tab[2];
			
			pers[cpt-1].gender = tab[3][0];
			
			nb_sep = 0;
			
			fonct(tab[4],&time,&nb_sep,'/');
			
			if(nb_sep != 0) {
				pers[cpt-1].birth.time.day = atoi(time[0]);
				
				pers[cpt-1].birth.time.month = atoi(time[1]);
				
				pers[cpt-1].birth.time.year = atoi(time[2]);
			} else {
				pers[cpt-1].birth.time.day = 0;
				
				pers[cpt-1].birth.time.month = 0;
				
				pers[cpt-1].birth.time.year = 0;
			}
			
			nb_sep = 0;
			
			pers[cpt-1].birth.location = tab[5];
			
			fonct(tab[6],&time,&nb_sep,'/');
			
			if(nb_sep != 0) {
				pers[cpt-1].death.time.day = atoi(time[0]);
				
				pers[cpt-1].death.time.month = atoi(time[1]);
				
				pers[cpt-1].death.time.year = atoi(time[2]);
			} else {
				pers[cpt-1].death.time.day = 0;
				
				pers[cpt-1].death.time.month = 0;
				
				pers[cpt-1].death.time.year = 0;
			}
			
			nb_sep = 0;
			
			pers[cpt-1].death.location = tab[7];
			
			father[cpt-1].surname = suppr(tab[8]);
			
			father[cpt-1].name = suppr(tab[9]);
			
			mother[cpt-1].surname = suppr(tab[10]);
			
			mother[cpt-1].name = suppr(tab[11]);
		}
		cpt++;
	}
	
	fclose(csv);
    
    cpt-- ;
            
    printf("\nLe fichier a permis le traitement de %d individus \n",cpt);
    
    if(ligne != NULL) {
		free(ligne);
		ligne = NULL;
	}
    
    for(int i = 0;i < cpt; i++) {
		
		newIndividual(pers[i],ptrtete,NULL,NULL);
	}

	//afficheIndiv(*ptrtete);
	
	individual *boucle = *ptrtete ;
	int k = cpt-1 ;
	while (boucle != NULL) {
		
		//printf("|%s| |%s|$\n",father[k].surname,father[k].name);
		if(rechercheIndiv(father[k].surname,father[k].name,pers[k].num_gen+1,*ptrtete) != NULL) {
			boucle->father = rechercheIndiv(father[k].surname,father[k].name,pers[k].num_gen+1,*ptrtete);
		}
		
		//printf("|%s| |%s| |%d| |%d|\n",mother[k].surname,mother[k].name,longChaine(mother[k].surname),longChaine(mother[k].name));
		if(rechercheIndiv(mother[k].surname,mother[k].name,pers[k].num_gen+1,*ptrtete) != NULL) {
			boucle->mother = rechercheIndiv(mother[k].surname,mother[k].name,pers[k].num_gen+1,*ptrtete);
		}
		
		k--;
		boucle = boucle->next ;
	}

}


void ecris_indiv(individual *ptrTete,char *nomFichier) {
	FILE* csv;
	individual *ptr = ptrTete;
	char **ligne = NULL;
	char *enTete = NULL;
	char *part1 = NULL;
	char *part2 = NULL;
	char *part3 = NULL;
	char *father = NULL;
	char *mother = NULL;
	int nb_indiv = 0;
	int cpt = 0;
	
	nb_indiv = compteIndiv(ptr);
	
	cpt = nb_indiv;
	
	ligne = (char**)malloc(sizeof(char*)*nb_indiv);
	
	enTete = (char*)malloc(sizeof(char)*500);
	part1 = (char*)malloc(sizeof(char)*500);
	part2 = (char*)malloc(sizeof(char)*500);
	part3 = (char*)malloc(sizeof(char)*500);
	father = (char*)malloc(sizeof(char)*500);
	mother = (char*)malloc(sizeof(char)*500);
	
	sprintf(enTete,"%d;nom;prenom;Genre;date_N;lieu_N;date_D;lieu_D;nom_P;prenom_P;nom_M;prenom_M\n",nb_indiv);
	
	
	
	while(ptr != NULL) {
		
		if(ptr->infos.num_gen == 0) {
			
			sprintf(part1,";%s;%s;%c;",ptr->infos.surname,ptr->infos.name,ptr->infos.gender);
			
		} else {
			
			sprintf(part1,"%d;%s;%s;%c;",ptr->infos.num_gen,ptr->infos.surname,ptr->infos.name,ptr->infos.gender);
			
		}
		
		if(dateNulle(ptr->infos.birth.time) == 0) {
			
			sprintf(part2,";%s;",ptr->infos.birth.location);
			
		} else {
			
			sprintf(part2,"%d/%d/%d;%s;",ptr->infos.birth.time.day,ptr->infos.birth.time.month,ptr->infos.birth.time.year,ptr->infos.birth.location);
			
		}
		
		if(dateNulle(ptr->infos.death.time) == 0) {
			
			sprintf(part3,";%s;",ptr->infos.death.location);
			
		} else {
			
			sprintf(part3,"%d/%d/%d;%s;",ptr->infos.death.time.day,ptr->infos.death.time.month,ptr->infos.death.time.year,ptr->infos.death.location);
			
		}
		
		if(ptr->father != NULL) {
			
			sprintf(father,"%s;%s;",ptr->father->infos.surname,ptr->father->infos.name);
			
		} else {
			
			strcpy(father,";;");
		}
		
		if(ptr->mother != NULL) {
			
			sprintf(mother,"%s;%s\n",ptr->mother->infos.surname,ptr->mother->infos.name);
			
		} else {
			
			strcpy(mother,";\n");
			
		}
		
		ligne[cpt-1] = (char*)malloc(sizeof(char)*(strlen(part1)+strlen(part2)+strlen(part3)+strlen(father)+strlen(mother)+1));
		
		sprintf(ligne[cpt-1],"%s%s%s%s%s",part1,part2,part3,father,mother);
		
		ptr = ptr->next;
		cpt--;
	}
	
	csv = fopen(nomFichier, "wt");
	
	printf("fichier identifié, écriture ...\n");
	
	fprintf(csv,"%s",enTete);
	
	for(int i = 0;i < nb_indiv; i++) {
		
		fprintf(csv,"%s",ligne[i]);
		
	}
	
	fclose(csv);
	
	
	
	if(csv != NULL){
		free(csv);
		csv = NULL;
	}
}


// recursivité pour rechercher les parents
// rechercher les individu sur pere 
// teste sur une 100 n d'indiv
// rejouter si existe pas 
// possibilité d'evolution a dire a l'exam final */
