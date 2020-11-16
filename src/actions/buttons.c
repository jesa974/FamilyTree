#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "../../include/aide.h"
#include "../../include/chaine.h"
#include "../../include/mariage.h"
#include "../../include/createIndividual.h"
#include "../../include/childLink.h"
#include "../../include/weddLink.h"

#include "../../include/My_GL.h"
#include "../../include/arbre.h"
#include "../../include/diagramme.h"
#include "../../include/statistique.h"
#include "../../include/texture.h"
#include "../../include/carto.h"
#include "../../include/ville.h"
#include "../../include/interface.h"
#include "../../include/buttons.h"

#define RAYON 8


void btnScreenshot(int lF, int hF){
	savePNGPicture("../ressources/output.bmp", lF, hF);
	printf("picture saved output.bmp saved in Ressources folder !\n");
	//Appel systeme pour ouvrir l'image
	system("eog ../ressources/output.bmp &");
}

void btnLocaliser(individual *id, OBJ3D* pinedCity, char* LastCity){
	if(id != NULL){
		printf("🌍 Localising the individual... please wait ... 🌍\n");
		//Verification de l'allocation mémoire
		if(pinedCity != NULL) {
			//Verification afin de ne pas traiter deux fois de suite une même ville
			if( strcmp( id->infos.birth.location, LastCity) || pinedCity->x== 0){
				*pinedCity = placeVille(id->infos.birth.location, RAYON-2);
			}
			else{
				printf("City of %s already pinned !\n",LastCity);
			}
			strcpy(LastCity, id->infos.birth.location);
		}
	}
}

void btnGoogleMaps(individual *id){
	if(id !=NULL && strcmp(id->infos.birth.location, "") != 0){
		char nav_loc[200];
		sprintf(nav_loc, "x-www-browser https://www.google.com/maps/place/%s", id->infos.birth.location);
		system(nav_loc);
	}
	else{
		char nav_loc[200];
		sprintf(nav_loc, "x-www-browser https://www.google.com/maps/");
		system(nav_loc);
	}
}


