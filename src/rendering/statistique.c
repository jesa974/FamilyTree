#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "../../include/aide.h"
#include "../../include/My_GL.h"
#include "../../include/interface.h"
#include "../../include/statistique.h"

void statistique(individual *ptrListe, int nb_indiv, char *domain){
	
	//Background
	col(200, 200, 200, 100);
	drawCarre(350, 25, 0, 1250, 1025, 0);
	
//Axe
	col(0, 0, 0, 100);
	
	//Abscisse
	drawCarre(380, 60, 0, 1200, 5, 0);
	
	float ecart_indiv = 1250/nb_indiv;//indicator
	for(int i=0; i<nb_indiv; i++){
		drawCarre(395 + ecart_indiv*i, 50, 0, 5, 10, 0);
		/*char is[3];
		sprintf(is, "%d", i+1);
		afficheChaine(is, 18, 2635 + ecart_indiv*i*6.6, 190);*/
	}
	
	
	//Ordonnée
	drawCarre(380, 60, 0, 5, 950, 0);
	
	//Display in depend of the domain
	if(strcmp(domain, "old") == 0){
		float ecart_age = 900/11;
		
		col(0, 0, 0, 100);
		for(int i=0; i<12; i++){
			drawCarre(370, 115 + ecart_age*i, 0, 10, 5, 0);
			/*char is[3];
			sprintf(is, "%d", (i+1)*10);
			afficheChaine(is, 17, 2470, 1200 + ecart_indiv*i*9.8);*/
		}
		for(int j=0; j<nb_indiv; j++){
			int old = getOld(ptrListe);
			
			col(255, 255, 255, 100);
			if(old != -1){
				drawDisque(10, 395 + ecart_indiv*j, 115 + old/11.5*ecart_age, 0, 20);
			}
			
			ptrListe = ptrListe->next;
		}
		
	}
	
}
