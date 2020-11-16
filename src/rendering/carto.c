#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "../../include/aide.h"
#include "../../include/My_GL.h"
#include "../../include/carto.h"
#include "../../include/ville.h"

void afficheVille(OBJ3D cityPt, char* nom_ville){
	glPushMatrix();
	if(strcmp(nom_ville,"")){
		glRotatef(-2.5,0,1,0);
		
		col(0,0,0,255);
		glTranslatef(0,0,cityPt.z);
		afficheString(SANS_SERIF_9,nom_ville,cityPt.x,cityPt.y+1);
		glTranslatef(0,0,-cityPt.z);
		
		col(255,0,0,100);
		glBegin(GL_LINES);
		glVertex3f(cityPt.x,cityPt.y,cityPt.z);
		glVertex3f(0.0,0.0,0.0);
		glEnd();
	}
	else{
		col(255,0,0,255);
		glRotatef(90,1,0,0);
		glTranslatef(0,0,7);
		
		afficheString(SANS_SERIF_9,"AUCUNE VILLE EN SELECTION",-2,0);
		
	}
	glPopMatrix();
}

OBJ3D placeVille(char* ville, int rayon){
		
		GPS city= trouveVilleGPS(ville);
		
		double lat = city.lats;
		double longitude = city.longs;
		double hauteur = 1.0;
		float x,y,z;

		//Constante d'applatissement de la terre, établie par le World Geodetic System 1984 : WGS84
		double FL = 1/298.257223563;

		double flatfn= (2 - FL)*FL;
        double funsq= (1 - FL)*(1 - FL);
        double g1;
        double g2;
        double sin_lat;

		double long_rad = longitude*M_PI/180;
		double lat_rad = lat*M_PI/180;

		sin_lat= sin( lat_rad );

        g1= rayon / sqrt( 1 - flatfn*sin_lat*sin_lat );
        g2= g1*funsq + hauteur;
        g1= g1 + hauteur;

        x= g1 * cos( lat_rad );
        y= x * sin( longitude*M_PI/180 );
        x= x * cos( long_rad );
       	z= g2 * sin_lat;

		OBJ3D cityPt;
		cityPt.x = x;
		cityPt.y = y;
		cityPt.z = z;

		return cityPt;
}

void displayCarte(Camera *Vision, OBJ3D pinedCity, char* ville, GLuint tex_planete, float rotx, float rotz){
	//Vision->center.x = pinedCity.x;
	//Vision->center.y = pinedCity.y;
	col(255,255,255, 255);
	glPushMatrix();
	glRotatef(-90,1,0,0);
	glRotatef(rotx,0,0,1);
	glDisable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	drawPlanete(5,30,tex_planete);

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

	afficheVille(pinedCity,ville);
	
	glPopMatrix();
}
