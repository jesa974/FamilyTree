#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include "../../include/aide.h"
#include "../../include/My_GL.h"
#include "../../include/arbre.h"

Indiv3D* getIndiv3DfromIndivListe(individual* indiv, Indiv3D** tableau, int* tabGen, int gen){
    for(int i=0; i< tabGen[gen]; i++){
        if(strcmp(indiv->infos.name,tableau[gen][i].prenom) == 0 && strcmp(indiv->infos.name,tableau[gen][i].prenom) == 0){
            return &(tableau[gen][i]);
        }
    }
    return NULL;
}


/**
 * @brief Inscrit les points dans un tableau d'individus
 * 
 * @param num numéro de l'individu
 * @param sample : échantillon = maximum d'invidu
 * @param jour : jour de naissance de l'individu
 * @param rayon : de la sphère
 */
OBJ3D init_fibonacci(int num, int sample, int jour, float rayon)
{
    double val = jour/31  * sample;
    double offset = 2.0/sample;
    double angle_or = PI*(3 - sqrt(5));

    double y = ((num * offset)-1)+ (offset / 2);
    double r = sqrt(1 - y*y);

    double phi = (fmod((num + val),sample)) * angle_or;
    y = y*rayon;
    double x = r * cos(phi) * rayon;
    double z = r * sin(phi) * rayon;

    OBJ3D coords;
    coords.x = (float) x;
    coords.y = (float) y;
    coords.z = (float) z;
    return coords;
}

/**
 * @brief Create a spheric tree object
 * 
 * @param x position
 * @param y position
 * @param z position
 * @param rayon radius
 * @param ptrTete Individual list
 * @param nb_individus max number of individuals
 */
Indiv3D** createArbre(float rayon, individual *ptrTete, int maxgen, int nb_indiv, int *nb_per_gen)
{
    individual *ptrCurrent = ptrTete;
    int jour_current;
    int traite = 0;
    int nbcases = maxgen;

    int* num_processed_gen = (int*) malloc(sizeof(int)*nbcases);
    Indiv3D **indiv_coords = malloc(sizeof(Indiv3D) * nbcases);
    //Initialisation du tableau dynamique
    for(int i=0; i< nbcases; i++){
        num_processed_gen[i] = 0;
        indiv_coords[i] = malloc(sizeof(Indiv3D)* nb_per_gen[i]);
    }

	//Parcours de la liste chainée
    while (ptrCurrent != NULL)
    {
        traite++;
        //Generation de l'individu:
        int indiv_gen = ptrCurrent->infos.num_gen;
        //Rayon : division du rayon demandé par la generation
        float radius = (float)rayon/(indiv_gen);
        //Max : nombre d'individus maximum de la generation concernée
        int sample = nb_per_gen[indiv_gen-1];

        //Jour de naissance : utile pour simuler une valeur aléatoire
        jour_current = ptrCurrent->infos.birth.time.day;
        
        int num_indiv = num_processed_gen[indiv_gen-1];
        
        //Appel de la fonction
        indiv_coords[indiv_gen-1][num_indiv].coords = init_fibonacci(num_indiv,sample,jour_current,radius);
        strcpy(indiv_coords[indiv_gen-1][num_indiv].nom,ptrCurrent->infos.surname);
        strcpy(indiv_coords[indiv_gen-1][num_indiv].prenom,ptrCurrent->infos.name);
        ptrCurrent = ptrCurrent->next;
        
        //On incrémente le nombre d'individus traités de la generation:
        num_processed_gen[indiv_gen-1]++;
    }

    free(num_processed_gen);
    num_processed_gen = NULL;
    
    return indiv_coords;

}

/**
 * @brief Fonction provisoire : renvoie le nombre d'individus d'une meme génération
 * 
 * @param gen : génération recherchée
 * @param ptrTete : liste chainée
 * @return int 
 */
int compte_indiv(int gen, individual* ptrTete){
    individual *current = ptrTete;
    int cpt = 0;
    int searchgen = 0;
     while (current != NULL){
        searchgen = current->infos.num_gen;
        if(searchgen == gen){
            cpt++;
        }

        current = current->next;
     }

    return cpt; 
}

//Gere le clique sur les differents individu
void cliqueSurIndiv(int x, int y)
{
}


void dessineLien(Indiv3D a, Indiv3D b){
	//On verifie si le processeur graphique supporte l'epaisseur
	GLfloat lineWidthRange[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE,lineWidthRange);

	//Si la taille de trait acceptée par la machine est limitée à 1.0 (AMD / OSX)
	if(lineWidthRange[1] == 1.0f){
		//Dessin de lignes épaisses adaptatif à tous types de processeurs graphiques:
		//Juxtaposition de deux triangles
		float e = 0.05;
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(a.coords.x-e, a.coords.y, a.coords.z);
			glVertex3f(a.coords.x+e, a.coords.y, a.coords.z);
			glVertex3f(b.coords.x-e, b.coords.y, b.coords.z);

			glVertex3f(b.coords.x-e, b.coords.y, b.coords.z);
			glVertex3f(b.coords.x+e, b.coords.y, b.coords.z);
			glVertex3f(a.coords.x+e, a.coords.y, a.coords.z);
		glEnd();
	}
	
	//Dessin de ligne classique
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glVertex3f(a.coords.x, a.coords.y, a.coords.z);
	glVertex3f(b.coords.x, b.coords.y, b.coords.z);
	glEnd();
}

void traceDescendance( int gen, Indiv3D* ancetre, Indiv3D* cur, Indiv3D** indiv_coords, int* tabGen, individual *id){
	//Si individu possède des enfants
	if(id->firstChild != NULL){
		childLink *enfant = id->firstChild;
		Indiv3D* enfant_pos;
		//Pour chaque enfant de l'individu
		while(enfant != NULL){
			//On rappelle la fonction
			enfant_pos = getIndiv3DfromIndivListe( enfant->child, indiv_coords, tabGen, gen-1);
			traceDescendance(gen-1, cur, enfant_pos, indiv_coords, tabGen, enfant->child);

				col(255, 127, 0, 100);
				dessineLien(*cur,*enfant_pos);

			enfant = enfant->next;
		}
	}
	//Sinon, on trace le lien entre les deux
	else{
		if(ancetre != NULL){
			col(0,200,0,200);
			dessineLien(*cur,*ancetre);
		}
	}
}

void traceLiensIndividu(individual *id, Indiv3D indiv_chosen, int* tabGen, Indiv3D **indiv_coords, int gen_chose ){
	if(id != NULL){
		Indiv3D* pere = NULL;
		Indiv3D* mere = NULL;
		
		if(id->father != NULL){
			pere = getIndiv3DfromIndivListe( id->father, indiv_coords, tabGen, gen_chose+1);
			if(pere !=NULL){
				col(0,0,255,100);
				dessineLien(indiv_chosen, *pere);
			}
		}

		if(id->mother != NULL){
			mere = getIndiv3DfromIndivListe( id->mother, indiv_coords, tabGen, gen_chose+1);
			if(mere !=NULL){
				col(255,0,0,100);
				dessineLien(indiv_chosen, *mere);
			}
		}
		if(id->firstChild != NULL){
			traceDescendance(gen_chose, NULL, &indiv_chosen, indiv_coords, tabGen, id);
		}
	}
}

void traceLiensMariages(individual *id, Indiv3D indiv_chosen, int* tabGen, Indiv3D **indiv_coords, int gen_chose){
	if(id != NULL){
		weddLink *ptr = id->firstUnion;
		while (ptr != NULL){
			individual* conjoint;
			
			if(id->infos.gender == 'F'){
				conjoint = id->firstUnion->mariage->info_wedding.husband;
			}
			else{
				conjoint = id->firstUnion->mariage->info_wedding.wife;
			}
			
			if(conjoint != NULL){
				Indiv3D* conjoint3D = getIndiv3DfromIndivListe(conjoint, indiv_coords, tabGen, conjoint->infos.num_gen-1);
				if(conjoint3D != NULL){
					//Affichage du lien (trait)
					col(255,0,255,150);
					dessineLien(indiv_chosen, *conjoint3D);
					
					//Surlignement du conjoint
					col(255, 0, 255, 150);
					drawSphere((*conjoint3D).coords.x, (*conjoint3D).coords.y, (*conjoint3D).coords.z, 0.3, 10, 0);
					
					char mariageName[60];
					char mariageDate[13];
					sprintf(mariageName,"mariage avec %s %s",
						conjoint->infos.name,
						conjoint->infos.surname
					);
					sprintf(mariageDate,"le %d/%d/%d",
						ptr->mariage->info_wedding.wedd.time.day,
						ptr->mariage->info_wedding.wedd.time.month,
						ptr->mariage->info_wedding.wedd.time.year
					);
					
					
					glPushMatrix();
					col(155,0,155,255);
					glTranslatef(1,0,(*conjoint3D).coords.z);
					afficheString(SANS_SERIF_8, mariageName,(*conjoint3D).coords.x, (*conjoint3D).coords.y);
					glTranslatef(0,-0.5,0);
					afficheString(SANS_SERIF_8, mariageDate,(*conjoint3D).coords.x, (*conjoint3D).coords.y);
					glPopMatrix();
					
				}
			}
			
			ptr = ptr->next;
		}
	}
		
}
















