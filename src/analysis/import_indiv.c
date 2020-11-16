#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/aide.h"
#include "../../include/createIndividual.h"
#include "../../include/import_indiv.h"

int convertisChaineInt(char* s){ //Verification des chiffres !
	//Table ASCII des chiffres : de 48 à 57 ('0'=48 ; '9' = 57)
	
	for(int i = 0; i < strlen(s)-1; i++){
		//On catche une erreur
		if( s[i] < 48 || s[i] > 57)
		{//Erreur :
			return 0;
			printf("Chaine non composee de chiffres : break\n");
		}
	}
	int n = atoi(s);
	return n;
}

void ajouteChar(char *s, char c){
	int lg = strlen(s);
	s[lg] = c;
	s[lg+1] = '\0';
}

void relie_parents( char* nomFichier, individual *ptrTete){
	char* name;
	char* surname;
	char* p_name;
	char* p_surname;
	char* m_name;
	char* m_surname;
	
	short num_gen;
	
	individual* indiv;
	
	printf("Fonction d'association des parents :\n");
// --- LECTURE DU FICHIER ----
	FILE* csv;
	int lig= 0;
	int separation = 0;
	char c = 0;
	char buffer[50];
	strcpy(buffer,"");
	
	csv = fopen(nomFichier, "rt");
	
	if(csv == NULL) { 
		printf("Le fichier n'existe pas! Fin.\n");
		return;
	}
	printf("fichier identifié, lecture...\n");
	//Compte du nombre de lignes
		while(c != EOF){
			c = getc(csv);
			
			if (c != '\n'){ //Tant que la ligne n'est pas terminee
				if(lig > 0){ //Dans le cas on l'on est pas sur la 1ere ligne (nom des colonnes)
						if( c == ';'){
							switch(separation){ //On s'adapte à la colonne à traiter
							case 0: //Colonne Generation
								num_gen = convertisChaineInt(buffer);
							break;
							
							case 1: //nom
								surname = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(surname, buffer);
							break;
							
							case 2: //prenom
								name = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(name, buffer);
							break;
							
							case 8: //nom_P
							//Recuperation de chaine
								p_surname = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(p_surname, buffer);
							break;
							
							case 9: //prenom_P
							//Recuperation de chaine
								p_name = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(p_name, buffer);
							break;
							
							case 10: //nom_M
								m_surname = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(m_surname, buffer);
							break;
							
							//Prenom M dans le cas '\n'
						}
						
						if(separation <3 || separation >7){
								printf("On a copié le buffer '%s' pour champ %d\n",buffer,separation);
								strcpy(buffer, "");
								printf("Test ok1\n");
							}
							printf("Test ok2\n");
						separation++;
						printf("separation = %d\n",separation);
					}
						
					else{ //Cas general
						if((separation <3 || separation >= 8) && c > 32){
							ajouteChar(buffer,c);
							printf("added '%c'\n",c);
						}
					}
				}
			}
			else{ //Fin de ligne = fin de lecture d'un individu
				if(lig>0){
					//prenom_M
					if(separation == 11) /*Ce n'est pas '?' */ {
						m_name = (char*) malloc(sizeof(char)*strlen(buffer));
						strcpy(m_name, buffer);
						printf("On a copié le buffer '%s' pour champ %d\n",buffer,separation);
						printf("Test ok4\n");
					}
					strcpy(buffer, "");
// -- Recherche des individus -- //		
					printf("recherche Indiv %s %s gen: %d\n", surname, name, num_gen);
					indiv = rechercheIndiv(surname, name, (int) num_gen, ptrTete);
					printf("Individu identifié : %s %s\n", indiv->infos.surname, indiv->infos.name);
					if(indiv->father !=NULL){
						printf("pere a chercher : %s %s\n", p_surname, p_name);
						indiv->father = rechercheIndiv(p_surname, p_name, (int) num_gen+1, ptrTete);
						printf("pere identifié : %s %s\n", indiv->father->infos.surname, indiv->father->infos.name);
					}
					if(indiv->mother !=NULL){
						indiv->mother = rechercheIndiv(m_surname, m_name, (int) num_gen+1, ptrTete);
						printf("mere identifié : %s %s\n", indiv->mother->infos.surname, indiv->mother->infos.name);
					}
					
				}
				lig++;
				separation = 0;
			}
		}
		fclose(csv);

}

individual* importer_liste_indiv(char * nomfichier){
	individual *ptrTete = NULL;
	
	FILE* csv;
	int lig= 0;
	int separation = 0;
	char c = 0;
	char buffer[50];
	char buffer_date[5];
	strcpy(buffer_date,"");
	strcpy(buffer,"");
	int cptDate=0 ;
	
	csv = fopen(nomfichier, "rt");
	
	if(csv == NULL) { 
		printf("Le fichier n'existe pas! Fin.\n");
		return NULL;
	}
	
	indivData donnees;
	individual pere, mere;
	
	//printf("fichier identifié, lecture...\n");
	//Compte du nombre de lignes
		while(c != EOF){
			c = getc(csv);

			if (c != '\n'){ //Tant que la ligne n'est pas terminee
				if(lig > 0){ //Dans le cas on l'on est pas sur la 1ere ligne (nom des colonnes)
						if( c == ';'){
							switch(separation){ //On s'adapte à la colonne à traiter
							case 0: //Colonne Generation
							//Recuperation d'un short (numgen)
								donnees.num_gen = convertisChaineInt(buffer);
							break;
							
							case 1: //Colonne nom
							//Recuperation de chaine
								donnees.surname = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(donnees.surname, buffer);
							break;
							
							case 2: //Colonne Prenom
							//Recuperation de chaine
								donnees.name = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(donnees.name, buffer);
							break;
							
							case 3: //Colonne Genre
							//Recuperation de char (on recupere le 1er caractere du buffer)
								donnees.gender = buffer[0];
							break;
							
							case 4: //Colonne date_N
							//Recuperation d'entier Par etapes : on a une chaine avec 19/02/1999 que l'on souhaite decouper puis traiter
							cptDate = 0;
							if(strlen(buffer)>2){ //Date connue
								for(int i=0; i < strlen(buffer); i++){
									if(buffer[i] != '/'){
										ajouteChar(buffer_date, buffer[i]);
									}
									else{ //Cas ou c = '/'
										if(cptDate == 0){ //Jour
											donnees.birth.time.day = (short) convertisChaineInt(buffer_date);
										}
										if(cptDate == 1){ //Mois
											donnees.birth.time.month = (short) convertisChaineInt(buffer_date);
										}
										strcpy(buffer_date,"");
										cptDate++;
									}
									
								}
								if(cptDate == 2){ //Annee
											donnees.birth.time.year = (short) convertisChaineInt(buffer_date);
											//printf("retour annee: %d\n",donnees.birth.time.year);
								}
								strcpy(buffer_date,"");
							}
							
							break;
							
							case 5: //Colonne lieu_N
							//Recuperation de chaine
								donnees.birth.location = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(donnees.birth.location, buffer);
							break;
							
							case 6: //Colonne date_D
							//Recuperation d'entier Par etapes : on a une chaine avec 19/02/1999 que l'on souhaite decouper puis traiter
							cptDate = 0;
							if(strlen(buffer) >2){
								for(int i=0; i < strlen(buffer); i++){
									if(buffer[i] != '/'){
										ajouteChar(buffer_date, buffer[i]);
									}
									else{
										if(cptDate == 0){ //Jour
											donnees.death.time.day = (short) convertisChaineInt(buffer_date);
										}
										if(cptDate == 1){ //Mois
											donnees.death.time.month = (short) convertisChaineInt(buffer_date);
										}
										strcpy(buffer_date,"");
										cptDate++;
									}
									if(cptDate == 2){ //Annee
											donnees.death.time.year = (short) convertisChaineInt(buffer_date);
									}
								}
								strcpy(buffer_date,"");
							}
							break;
							
							case 7: //Colonne lieu_D
							//Recuperation de chaine
								donnees.death.location = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(donnees.death.location, buffer);
							break;
							
							case 8: //Colonne nom_P
							//Recuperation de chaine
								pere.infos.surname = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(pere.infos.surname, buffer);
							break;
							
							case 9: //Colonne prenom_P
							//Recuperation de chaine
								pere.infos.name = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(pere.infos.name, buffer);
							break;
							
							case 10: //Colonne nom_M
							//Recuperation de chaine
								mere.infos.surname = (char*) malloc(sizeof(char)*strlen(buffer));
								strcpy(mere.infos.surname, buffer);
							break;
							
							//Prenom M dans le cas '\n'

							default:
							break;
							
							}
							//printf("chaine copiee : '%s' [%d]\n",buffer, separation);
							strcpy(buffer, "");
							separation++;
						}
						
						else{ //Cas general
							if(c >32){
							ajouteChar(buffer,c);}
						}
					}
				}
			else{ //Fin de ligne = fin de lecture d'un individu
				//Colonne prenom_M
				//Recuperation de chaine
				if(lig>0){
					if(separation == 11) {
						mere.infos.name = (char*) malloc(sizeof(char)*strlen(buffer));
						strcpy(mere.infos.name, buffer);
					}
					newIndividual(donnees, &ptrTete, &pere, &mere);
				}
				lig++; //on incremente le nombre de lignes lues
				separation = 0;
				strcpy(buffer, "");
				donnees.birth.time.day = 0;
				donnees.birth.time.month = 0;
				donnees.birth.time.year = 0;
				donnees.death.time.day = 0;
				donnees.death.time.month = 0;
				donnees.death.time.year = 0;
			}
		}
		fclose(csv);
            
         //printf("\nLe fichier a permis le traitement de %d individus \n",lig-1);
         
	return ptrTete;
}

/*
void ecris_fichier_indiv(char * nomSortie, individual *ptrTete, int nb){
	
	FILE* csv;
	csv = fopen(nomSortie, "wt");
	individual *ptr = ptrTete;
	int cpt = 1;
	
	if(csv != NULL)
	{
		//Ecriture de la 1ere ligne:
		fprintf(csv,"%d;",nb);
		fprintf(csv, "nom;prenom;Genre;date_N;lieu_N;date_D;lieu_D;nom_P;prenom_P;nom_M;prenom_M\n");
		printf("1ere ligne ecrite OK\n");
		
		while(ptr !=NULL){
			printf("Tentative indiv %d\n",cpt);
			//On ecris les donnees dans le fichier
			fprintf(csv,"%d;",ptr->infos.num_gen);
			fprintf(csv,"%s;",ptr->infos.surname);
			fprintf(csv,"%s;",ptr->infos.name);
			fprintf(csv,"%c;",ptr->infos.gender);
			fprintf(csv,"%d/%d/%d;",ptr->infos.birth.time.day, ptr->infos.birth.time.month, ptr->infos.birth.time.year);
			fprintf(csv,"%s;",ptr->infos.birth.location);
			fprintf(csv,"%d/%d/%d;",ptr->infos.death.time.day, ptr->infos.death.time.month, ptr->infos.death.time.year);
			fprintf(csv,"%s;",ptr->infos.death.location);
			fprintf(csv,";;;\n");
			
			fprintf(csv,"%s;",ptr->father->infos.surname);
			printf("surname_f OK : %s\n",ptr->father->infos.surname);
			fprintf(csv,"%s;",ptr->father->infos.name);
			printf("name_f OK\n");
			fprintf(csv,"%s;",ptr->mother->infos.surname);
			printf("surname_m OK\n");
			fprintf(csv,"%s\n",ptr->mother->infos.name);
			printf("name_m OK\n");
			 
			//Fin de traitement de l'individu
			ptr = ptr->next; //On passe au suivant
			cpt++;
		}

		fclose(csv);
		printf("Fichier ecris avec %d individus\n",cpt-1);
	}
	else{
		printf("Fichier csv NULL\n");
	}
}

int main(int argc, char **argv) {
	individual *I;
	if(argc > 1)
	{
		printf("Lecture de %s...\n",argv[1]);
		I = importer_liste_indiv(argv[1]);
		afficheIndiv(I);
		//relie_parents(argv[1], I);
		ecris_fichier_indiv("out.csv", I, 21);
	}
	else{
		printf("Merci de stipuler un nom de fichier en argument (ex: ./app nom_fichier.csv) \n");
	}

	return 0;
}
*/


