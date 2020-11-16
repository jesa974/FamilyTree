#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "../../include/aide.h"
#include "../../include/chaine.h"
#include "../../include/My_GL.h"
#include "../../include/interface.h"
#include "../../include/createIndividual.h"

#define POLICE GLUT_BITMAP_HELVETICA_18
typedef enum panState{INDIV, INSERT, EDIT, DELETE }panState;
typedef enum areaSel{NONE, SURNAME, NAME, AGE, BIRTH_DATE, BIRTH_LOCATION, DEATH_DATE, DEATH_LOCATION, GENDER, GEN, FATHER, MOTHER, CHILD}areaSel;

void indivEtDeletePanel(individual *id, Bouton *boutons, char tabInfos[NB_INFOS][150], individual *ptrListe, short num);
void insertEditPanel(individual *indiv_to_insert, int **areaSelected, char *value, individual *ptrListe, Bouton *boutons, char tabInfos[NB_INFOS][150], short panel);

void leftPane(individual* id_chose, Couleur rgb, int cadrex, int cadrey, int large, int haut){
	int tailleBonhomme = 40;
	
	//Background
	col(250, 250, 250, 100);
	rectanglePlein(cadrex, cadrey, cadrex+large, cadrey+haut);
	
	//------------------Avatar---------------
	
	//Background
	col(50, 50, 50, 255);//Border
	drawDisque(tailleBonhomme*1.75, cadrex+large/2.1, cadrey+2*haut/2.5, 180, 1);
	col(240, 240, 240, 255);//Fill
	drawDisque(tailleBonhomme*1.7, cadrex+large/2.1, cadrey+2*haut/2.5, 180, 1);
	
	//If girl : long hair
	col(rgb.r, rgb.g, rgb.b, 255);
	if(id_chose != NULL && id_chose->infos.gender == 'F'){
		drawDisque(tailleBonhomme/1.5, cadrex+large/2.1,  cadrey+2*haut/2.7 + tailleBonhomme*1.7, 180, 2);
		rectanglePlein(cadrex+large/2-tailleBonhomme/1.07, cadrey+2*haut/2.9 + tailleBonhomme*2.5, cadrex+large/3+tailleBonhomme*2.2, cadrey+2*haut/3 + tailleBonhomme*2 + tailleBonhomme/6);
	}
		
	col(0, 0, 0, 255);
	drawDisque(tailleBonhomme, cadrex+large/2.1, cadrey+2*haut/2.7, 180, 2);//Body
	
	drawCercle(0, tailleBonhomme/2, cadrex+large/2.1, cadrey+2*haut/2.7 + tailleBonhomme*1.6,50);//Head
	
	col(rgb.r, rgb.g, rgb.b, 255);//Random hair color
	drawDisque(tailleBonhomme/2, cadrex+large/2.1,  cadrey+2*haut/2.7 + tailleBonhomme*1.6, 180, 2);//Hair
	
	
	//------------------Textes----------------
	
	//Traits décoratifs
	col(rgb.r, rgb.g, rgb.b, 255);
	rectanglePlein(cadrex+large/25, cadrey+2*haut/2.55, cadrex+large/3.7, cadrey+2*haut/2.53);//pic
	rectanglePlein(cadrex+large/1.45, cadrey+2*haut/2.55, cadrex+large/1.05, cadrey+2*haut/2.53);//pic
	
	cadrey = 3*hF/20;
		
	col(0, 0, 0, 255);
	rectanglePlein(cadrex+large/30, cadrey+2*haut/3.3, cadrex+large/25, cadrey+2*haut/35);//Info
}

//--------------------------IHM---------------------------------
void interface2d(int menu, int timerAnim, int lF, int hF, Bouton* boutons, char* search, individual *id, Couleur rgb, int panelActive, int *areaSelected, char *value, individual *ptrListe, individual *indiv_to_insert, char tabInfos[NB_INFOS][150])
{	
	int large = 2*lF/6;
	int haut = 4*hF/5;
	int cadrex = lF/70;
	int cadrey = 2*hF/20;
	
	//-------Always active-------
	menuPanel(menu, timerAnim, lF, hF, id, boutons, search);
	
	panState etat = panelActive;
	
	if(etat != -1){
		leftPane(id, rgb,cadrex,cadrey,large,haut);
	
	
		//Initialisation of informations
		
		col(0, 0, 0, 255);
		//Attribute the value and display it
		switch(etat){
			case INDIV:
				if(id != NULL)
					indivEtDeletePanel(id, boutons, tabInfos, ptrListe, 1);
			break;
			
			case INSERT:
				if(indiv_to_insert != NULL)
					insertEditPanel(indiv_to_insert, &areaSelected, value, ptrListe, boutons, tabInfos, 1);
			break;
			
			case EDIT:
				if(indiv_to_insert != NULL)
					insertEditPanel(indiv_to_insert, &areaSelected, value, ptrListe, boutons, tabInfos, 2);
			break;

			case DELETE:
				if(id != NULL)
					indivEtDeletePanel(id, boutons, tabInfos, ptrListe, 2);
			break;
			
			default:
			break;
		}
	
		//--------------------------Display-----------------------
		
		float tabY[NB_INFOS] = {3.7, 4, 4.35, 4.8, 5.3, 5.9, 6.65, 7.6, 9, 11, 14.2, 20};
		for(int i=0; i<NB_INFOS; i++){
			afficheString(POLICE, tabInfos[i], cadrex+large/12, cadrey+2.37*haut/tabY[i]);//Name
		}
		
		//Affichage du bouton de fermeture
		afficheBtn(&boutons[13]);
	}
	
	
}

void indivEtDeletePanel(individual *id, Bouton *boutons, char tabInfos[NB_INFOS][150], individual *ptrListe, short num){
	short day, month, year;
			
	col(0,0,0,255);
		
	//Surname
	sprintf(tabInfos[0], "Surname : %s", id->infos.surname);
	
	//Name
	sprintf(tabInfos[1], "Name : %s", id->infos.name);
	
	//Age
	if(getOld(id) != 0){
		sprintf(tabInfos[2], "Age : %d", getOld(id));
	}
	else{
		strcpy(tabInfos[2], "?");
	}
	
	//Birth date
	day = id->infos.birth.time.day;
	month = id->infos.birth.time.month;
	year = id->infos.birth.time.year;
	sprintf(tabInfos[3], "Birth date : %u/%u/%u", day, month, year);
	
	//Birth location
	if(strcmp(id->infos.birth.location,"")){
		sprintf(tabInfos[4], "Birth location : %s", id->infos.birth.location);
		if(num == 1) afficheBtn(&(boutons[9]));//Bouton localiser
	}
	else{
		if(num == 1) afficheBtnGrise(&(boutons[9]));
	}
	
	//Death date
	if(id->infos.death.time.day != 0){
		day = id->infos.death.time.day;
		month = id->infos.death.time.month;
		year = id->infos.death.time.year;
		sprintf(tabInfos[5], "Death date : %u/%u/%u", day, month, year);
	}else{
		strcpy(tabInfos[5], "Death date : ?");
	}
	
	//Death location
	sprintf(tabInfos[6], "Death location : %s", id->infos.death.location);
	
	//Gender
	sprintf(tabInfos[7], "Gender : %c", id->infos.gender);
	
	//Generation
	sprintf(tabInfos[8], "Generation : %d", id->infos.num_gen);
	
	//Father
	if(id->father != NULL){
		sprintf(tabInfos[9], "Father : %s %s", id->father->infos.surname, id->father->infos.name);
		if(num == 1) afficheBtn(&(boutons[5]));//Father button
	}
	else{
		if(num == 1) afficheBtnGrise(&(boutons[5]));//Father button
		strcpy(tabInfos[10], "Father :");
		col(0,0,0,255);
	}
	
	//Mother
	if(id->mother != NULL){
		sprintf(tabInfos[10], "Mother : %s %s", id->mother->infos.surname, id->mother->infos.name);
		if(num == 1) afficheBtn(&(boutons[6]));//Mother button
	}
	else{
		if(num == 1) afficheBtnGrise(&(boutons[6]));//Mother button
		strcpy(tabInfos[10], "Mother :");
		col(0,0,0,255);
	}
	
	//Child
	if(id->firstChild != NULL){
		sprintf(tabInfos[11], "Child : %s %s", id->firstChild->child->infos.surname, id->firstChild->child->infos.name);
		if(num == 1) afficheBtn(&(boutons[7]));//Children button
	}
	else{
		if(num == 1) afficheBtnGrise(&(boutons[7]));//Children button
		strcpy(tabInfos[11], "Child :");
		col(0,0,0,255);
	}
	//Child
	if(id->firstUnion != NULL){
		if(num == 1) afficheBtn(&(boutons[8]));//Latest husband button
	}
	else{
		if(num == 1) afficheBtnGrise(&(boutons[8]));
		col(0,0,0,255);
	}
	
	if(num == 2){
		afficheBtn(&(boutons[12]));//Apply Delete button
	}
		
}

void insertEditPanel(individual *indiv_to_insert, int **areaSelected, char *value, individual *ptrListe, Bouton *boutons, char tabInfos[NB_INFOS][150], short panel){
	individual *father = NULL;
	individual *mother = NULL;
	individual *child = NULL;
	childLink *childL = NULL;
	char **info;
	int nb;
	
	//Selected and edit the area selected
	if(**areaSelected && strcmp(value, "") != 0){
		areaSel sel= **areaSelected;
		char day[3], month[3], year[5];
		
		switch(sel){
			
			case NONE:
				break;
					 
			case SURNAME:
				indiv_to_insert->infos.surname = value;
				sprintf(tabInfos[0], "Surname : %s", value);
				break;
					
			case NAME:
				indiv_to_insert->infos.name = value;
				sprintf(tabInfos[1], "Name : %s", value);
				break;
					
			case AGE:
				break;
					
			case BIRTH_DATE:
				day[0] = value[0]; day[1] = value[1];
				
				if(atoi(day) > -1 && atoi(day) < 32){//Day 2 char
					indiv_to_insert->infos.birth.time.day = atoi(day);
					sprintf(tabInfos[3], "Birth date : %s", day);
						
					if(longChaine(value) > 2){//Month > 2 char
						sprintf(month, "%c%c", value[2], value[3]);
						if(atoi(month) > -1 && atoi(month) < 13){
							indiv_to_insert->infos.birth.time.month = atoi(month);
							sprintf(tabInfos[3], "Birth date : %s/%s", day, month);
								
							if(longChaine(value) > 4){//Year > 4 char
								time_t t = time(NULL);
								struct tm tm = *localtime(&t);
								sprintf(year, "%c%c%c%c", value[4], value[5], value[6], value[7]);
								if(atoi(year) > -1 && atoi(year) < tm.tm_year + 1900 + 1){
									indiv_to_insert->infos.birth.time.year = atoi(year);
									sprintf(tabInfos[3], "Birth date : %s/%s/%s", day, month, year);
								}
								else{
									memset(value, 0, sizeof(value)+1);
									strcpy(tabInfos[3], "Birth date : ");
								}
							}
						}
						else{
							memset(value, 0, sizeof(value)+1);
							strcpy(tabInfos[3], "Birth date : ");
						}
					}				
				
				}
				else{
					memset(value, 0, sizeof(value)+1);
					strcpy(tabInfos[3], "Birth date : ");
				}
				break;
					
			case BIRTH_LOCATION:
				indiv_to_insert->infos.birth.location = value;
				sprintf(tabInfos[4], "Birth location : %s", value);
				break;
				
			case DEATH_DATE:
				sprintf(day, "%c%c", value[0], value[1]);
				
				if(atoi(day) > -1 && atoi(day) < 32){//Day 2 char
					indiv_to_insert->infos.death.time.day = atoi(day);
					sprintf(tabInfos[5], "Death date : %s", day);
						
					if(longChaine(value) > 2){//Month > 2 char
						sprintf(month, "%c%c", value[2], value[3]);
						if(atoi(month) > -1 && atoi(month) < 13){
							indiv_to_insert->infos.death.time.month = atoi(month);
							sprintf(tabInfos[5], "Death date : %s/%s", day, month);
								
							if(longChaine(value) > 4){//Year > 4 char
								time_t t = time(NULL);
								struct tm tm = *localtime(&t);
								sprintf(year, "%c%c%c%c", value[4], value[5], value[6], value[7]);
								if(atoi(year) > -1 && atoi(year) < tm.tm_year + 1900 + 1){
									indiv_to_insert->infos.death.time.year = atoi(year);
									sprintf(tabInfos[5], "Death date : %s/%s/%s", day, month, year);
								}
								else{
									memset(value, 0, sizeof(value)+1);
									strcpy(tabInfos[5], "Death date : ");
								}
							}
						}
						else{
							memset(value, 0, sizeof(value)+1);
							strcpy(tabInfos[5], "Death date : ");
						}
					}				
				
				}
				else{
					memset(value, 0, sizeof(value)+1);
					strcpy(tabInfos[5], "Death date : ");
				}
				break;
					
			case DEATH_LOCATION:
				indiv_to_insert->infos.death.location = value;
				sprintf(tabInfos[6], "Death location : %s", value);
				break;
					
			case GENDER:
				if(value[0] == 'F' || value[0] == 'M'){
					indiv_to_insert->infos.gender = value[0];
					sprintf(tabInfos[7], "Gender : %c", value[0]);
				}
				else{
					memset(value, 0, sizeof(value)+1);
					strcpy(tabInfos[7], "Gender : ");
				}
				break;
					
			case GEN:
				indiv_to_insert->infos.num_gen = atoi(value);
				sprintf(tabInfos[8], "Generation : %s", value);
				break;
				
			case FATHER:
				if(indiv_to_insert->infos.num_gen != 0){
					sprintf(tabInfos[9], "Father : %s", value);
					
					fonct(value, &info, &nb, ' ');
					
					if(strcmp(info[0], "") && strcmp(info[1], "")){
						
						father = rechercheIndiv(info[0], info[1], indiv_to_insert->infos.num_gen+1, ptrListe);
						
						if(father == NULL){
							father = malloc(sizeof(individual));
							
							father->infos.surname = info[0];
							father->infos.name = info[1];
							father->infos.num_gen = indiv_to_insert->infos.num_gen+1;
							father->father=NULL;
							father->mother=NULL;
						}
						
						newIndividual(father->infos, &ptrListe, NULL, NULL);
						indiv_to_insert->father=father;
					}
					
				}
				else{
					strcpy(tabInfos[9], "Father : no generation enter !");
					memset(value, 0, sizeof(value)+1);
				}
				break;
					
			case MOTHER:
				if(indiv_to_insert->infos.num_gen != 0){
					sprintf(tabInfos[10], "Mother : %s", value);
					
					fonct(value, &info, &nb, ' ');
					
					if(strcmp(info[0], "") && strcmp(info[1], "")){
						
						mother = rechercheIndiv(info[0], info[1], indiv_to_insert->infos.num_gen+1, ptrListe);
						
						if(mother == NULL){
							mother = malloc(sizeof(individual));
							
							mother->infos.surname = info[0];
							mother->infos.name = info[1];
							mother->infos.num_gen = indiv_to_insert->infos.num_gen+1;
							mother->father=NULL;
							mother->mother=NULL;
						}
						
						newIndividual(mother->infos, &ptrListe, NULL, NULL);
						indiv_to_insert->mother=mother;
					}
					
				}
				else{
					strcpy(tabInfos[10], "Mother : no generation enter !");
					memset(value, 0, sizeof(value)+1);
				}
				break;
					
			case CHILD:
				if(indiv_to_insert->infos.num_gen > 1){
						sprintf(tabInfos[11], "Child : %s", value);
						
						fonct(value, &info, &nb, ' ');
						
						if(strcmp(info[0], "") && strcmp(info[1], "")){
							
							child = rechercheIndiv(info[0], info[1], indiv_to_insert->infos.num_gen-1, ptrListe);
							
							if(child == NULL){
								child = malloc(sizeof(individual));
								
								child->infos.surname = info[0];
								child->infos.name = info[1];
								child->infos.num_gen = indiv_to_insert->infos.num_gen-1;
								mother->father=NULL;
								mother->mother=NULL;
							}
							
							if(indiv_to_insert->infos.gender == 'F'){
								newIndividual(child->infos, &ptrListe, NULL, indiv_to_insert);
							}else{
								newIndividual(child->infos, &ptrListe, indiv_to_insert, NULL);
							}
							
							childL->child = child;
							if(indiv_to_insert->firstChild != NULL){
								indiv_to_insert->firstChild->next = childL;
							}
							else{
								indiv_to_insert->firstChild = childL;
							}
						}
						
					}
					else{
						strcpy(tabInfos[11], "Child : no generation entered !");
						memset(value, 0, sizeof(value)+1);
					}
			
				break;
				
		}
		
	}
	else{
		memset(value, 0, sizeof(value)+1);
	}
	
	if(panel == 1){//Insert panel
		afficheBtn(&(boutons[10]));//Apply Insert button
	}
	else{//Edit panel
		afficheBtn(&(boutons[11]));//Apply Edit button
	}
}

void menuPanel(int menu, int timerAnim, int lF, int hF, individual* indiv_chosen, Bouton boutons[7], char search[50]){
	float start = 2*lF/3;
	//Bouton capture d'ecran
	afficheBtn(&boutons[14]);
	
	if(!(menu)){
		
		if(timerAnim == -1){//If tab not pressed
			glColor3f(0.1016,0.3242,0.3008);
			rectanglePlein(lF - LARGEUR_FENETRE/30, 0,lF, hF);
		}
		else{//If tab pressed
			glColor3f(0.1398,0.4462,0.4140);
			rectanglePlein(start + timerAnim - LARGEUR_FENETRE/40, 0,start+timerAnim, hF);
			glColor3f(0.906,0.926,0.926);
			rectanglePlein(start+timerAnim, 0, lF, hF);
		}
	}
	else if(timerAnim == -1){
		//Background
		glColor3f(0.1398,0.4462,0.4140);
		rectanglePlein(start - LARGEUR_FENETRE/40, 0,2*lF/3, hF);
		
		//White Theme
		glColor3f(0.906,0.926,0.926);
		rectanglePlein(start,0,lF,hF);
		//Button search
		if(strcmp(search,"")){
			afficheBtn(&(boutons[0]));
		}
		else{
			afficheBtnGrise(&(boutons[0]));
		}
		
		//Buttons modifier et supprimer
		if(indiv_chosen !=NULL){
			afficheBtn(&(boutons[1]));
			afficheBtn(&(boutons[2]));
		}
		else{
			afficheBtnGrise(&(boutons[1]));
			afficheBtnGrise(&(boutons[2]));
		}
		
		//Button insert, delete...
		for(int i=3; i<5; i++){
			afficheBtn(&(boutons[i]));
		}
		
		//Button Process AI, save...
		for(int i=15; i<17; i++){
			afficheBtn(&(boutons[i]));
		}
		
		glColor3f(0.806,0.826,0.826);
		rectanglePlein(boutons[0].x1, 480, boutons[0].x2, 530);
		//Display string of search
		glColor3f(0,0,0);
		afficheString(POLICE,search,boutons[0].x1 + 10, 500);
	}
	else{//If tab pressed
		glColor3f(0.1398,0.4462,0.4140);
		rectanglePlein(lF- timerAnim - lF/40, 0,lF- timerAnim, hF);
		glColor3f(0.906,0.926,0.926);
		rectanglePlein(lF- timerAnim,0,lF,hF);
	}
}


//--------------------------Text--------------------------------
char *textField(char key, char *search, int numbers){
	if(numbers){
		if( (key >= 48 && key < 58) && strlen(search)< 50 ){ //Caracteres ASCII clavier
			char buffersearch[2];
			sprintf(buffersearch,"%c",key);
			strcat(search,buffersearch);
		}
	}
	else{
		if( (key == 32 || (key >= 65 && key <91) || (key >= 97 && key < 123)) && strlen(search)< 50 ){ //Caracteres ASCII clavier
			char buffersearch[2];
			sprintf(buffersearch,"%c",key);
			strcat(search,buffersearch);
		}
	}
	
	if(key == 8 && strlen(search) >= 1 ){ //Touches retour (supprimer la derniere lettre)
		char buffersearch[strlen(search)];
		strcpy(buffersearch, "");
		char chbuff[2];
		char ks;
		for(int i=0; i < strlen(search)-1; i++){
			ks = search[i];
			sprintf(chbuff, "%c",ks);
			strcat(buffersearch,chbuff);
		}
		strcpy(search,buffersearch);
	}
	
	return search;
}


void findAreaSelected(int panelActive, int x, int y, int lF, int hF, int *areaSelected){
	int large = 2*lF/6;
	int haut = 4*hF/5;
	int cadrex = lF/70;
	int cadrey = 2*hF/20;
	y = hF - y;
	int tmp = *areaSelected;
	
	if(panelActive){
		
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/3.7-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/3.7+15){
			*areaSelected = SURNAME; //1
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/4-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/4+15){
			*areaSelected = NAME; //2
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/4.35-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/4.35+15){
			*areaSelected = AGE; //3
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/4.8-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/4.8+15){
			*areaSelected = BIRTH_DATE; //4
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/5.3-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/5.3+15){
			*areaSelected = BIRTH_LOCATION; //5
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/5.9-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/5.9+15){
			*areaSelected = DEATH_DATE; //6
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/6.65-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/6.65+15){
			*areaSelected = DEATH_LOCATION; //7
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/7.6-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/7.6+15){
			*areaSelected = GENDER; //8
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/9-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/9+15){
			*areaSelected = GEN; //9
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/11-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/11+15){
			*areaSelected = FATHER; //10
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/14.2-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/14.2+15){
			*areaSelected = MOTHER; //11
		}
		if(x > cadrex+large/30 && y > cadrey+2.37*haut/20-3 && x < cadrex+large/1.05 && y < cadrey+2.37*haut/20+15){
			*areaSelected = CHILD; //12
		}
		
	}
	if(tmp != 0 && tmp != *areaSelected){
		*areaSelected = 0;
	}
	
}

int getOld(individual *id){	
	int yearDeath = id->infos.death.time.year;
	int yearBirth = id->infos.birth.time.year;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	if(yearDeath != 0 && yearBirth != 0){
		return yearDeath - yearBirth;
	}
	if(yearBirth != 0 && yearDeath == 0){
		return tm.tm_year + 1900 - yearBirth;
	}
	else{
		return 0;
	}
}


