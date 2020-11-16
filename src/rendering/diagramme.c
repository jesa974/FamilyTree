#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include "../../include/aide.h"
#include "../../include/My_GL.h"
#include "../../include/interface.h"
#include "../../include/diagramme.h"

#define POLICE GLUT_BITMAP_9_BY_15

//Display the gender's diagramme
void diagramme(individual *ptrListe, int nb_indiv){
	int nb_M = 0;
	
	//Calculate the numbers of man and woman
	for(int i=0; i<nb_indiv; i++){
		if(ptrListe->infos.gender == 'M'){
			nb_M++;
		}
		
		ptrListe = ptrListe->next;
	}
	drawDiagramme(500, 960, 550, 0, X, Y, 50, nb_M, nb_indiv);
	
	col(255, 255, 255, 100);
	char prct1s[5];
	sprintf(prct1s, "%.1f", nbToPrct(nb_M, nb_indiv));
	strcat(prct1s, "%");
	afficheString(POLICE, prct1s, 800, 2000);
	char prct2s[5];
	sprintf(prct2s, "%.1f", 100-nbToPrct(nb_M, nb_indiv));
	strcat(prct2s, "%");
	afficheString(POLICE, prct1s, 3500, 500);
	
	
}
