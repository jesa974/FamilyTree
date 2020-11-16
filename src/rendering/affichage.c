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

#define POLICE GLUT_BITMAP_HELVETICA_18
#define PI 3.14159

void couleurCourante(int r, int g, int b){
	glColor3f(r/255.f, g/255.f, b/255.f);
}

/* Cette fonction retourne un entier : vrai ou faux  si l'on se trouve (en x,y) dans le rectangle */
int isIn(float x1b,float x2b,float y1b,float y2b,float x,float y){
	if(x>= x1b && x<= x2b && y >= y1b && y <= y2b)
		return 1;
	else return 0;
}

int insideBtn(Bouton b, int x, int y){
	if(isIn(b.x1, b.x2, b.y1, b.y2,(float) x,(float) y))
			return 1;
	else return 0;
}

void afficheBtnGrise(Bouton *btn){
	/* Affichage */
	couleurCourante(87,132,128);

	float height = btn->y1 - btn->y2;
	float width = btn->x2 - btn-> x1;
	
	//Tracé
	rectanglePlein( btn->x1, hF - btn->y1, btn->x2, hF - btn->y2);
	//Label
	col(0,0,0,100);

	//Affichage bitmap : Moins de calcul, plus joli (moins permissif!)
	afficheString(POLICE,btn->label, btn->x1 + width/2 - tailleString(btn->label, POLICE)/2 , hF + height/2 - 9 - btn->y1);
}

void afficheBtn(Bouton *btn){
	int r, g, b;
	r = btn->color.r;
	g = btn->color.g;
	b = btn->color.b;
	/* Affichage */
	if(btn->clicked)
	//Couleur : cas cliqué
	couleurCourante(r-10,g-10,b-10);
	else if(btn->hovered)
	//Couleur : cas survolé
	couleurCourante(r+10,g+10,b+10);
	else
	//Couleur : cas classique
	couleurCourante(r,g,b);

	float height = btn->y1 - btn->y2;
	float width = btn->x2 - btn-> x1;
	
	//Tracé
	rectanglePlein( btn->x1, hF - btn->y1, btn->x2, hF - btn->y2);
	//Label
	glColor3f(0,0,0);

	//Affichage bitmap : Moins de calcul, plus joli (moins permissif!)
	afficheString(POLICE,btn->label, btn->x1 + width/2 - tailleString(btn->label, POLICE)/2 , hF + height/2 - 9 - btn->y1);
}

void afficheLigne(float x1, float y1, float z1, float x2, float y2, float z2){
	glBegin(GL_LINES);
	glVertex3f(x1,y1,z1);
	glVertex3f(x2,y2,z2);
	glEnd();
}

//----------------------------------Rond--------------------------------
void drawDiagramme(float r, float x, float y, float z, char axe1, char axe2, int sommets, int num, int nb_indiv){	
	float pctnum1 = nbToPrct(num, nb_indiv);
		
	col(255, 0, 0, 255);
	glBegin(GL_POLYGON);
	
	for (int i = 0; i < sommets; i++)
	{
		float theta = (2 * PI * i) / sommets;

		float c1 = r * cos(theta);
		float c2 = r * sin(theta);
		
		if(i > sommets*(pctnum1/100)){
			col(255, 255, 0, 255);
		}

		if ((axe1 == 'x' && axe2 == 'y') || (axe1 == 'y' && axe2 == 'x'))
			glVertex3f(x + c1, y + c2, z);
		if ((axe1 == 'x' && axe2 == 'z') || (axe1 == 'z' && axe2 == 'x'))
			glVertex3f(x + c1, y, z + c2);
		if ((axe1 == 'z' && axe2 == 'y') || (axe1 == 'y' && axe2 == 'z'))
			glVertex3f(x, y + c1, z + c2);
			
	}
	glEnd();
	
}

void drawDisque(float r, float x, float y, int precision, int decoupage)
{
	glPushMatrix();
	glTranslatef(x,y,0);
	glBegin(GL_POLYGON);
		for(int i=0; i<=precision/decoupage; i++){
			double angle= 2*M_PI * i/precision;
			double x = cos(angle) *r;
			double y = sin(angle) *r;
			glVertex2d(x,y);
		}
	glEnd();
	glPopMatrix();
}

void drawCercle(float rIn, float rOut, float x, float y, int segments)
{
	glPushMatrix();
	glTranslatef(x,y,0);
    GLUquadric* params = gluNewQuadric();
	gluDisk(params, rIn, rOut, segments, 1);
	gluDeleteQuadric(params);
	glPopMatrix();
}

void afficheString(void * font, char *text,int x,int y)
{
	glRasterPos2i(x, y);
	glutBitmapString(font, (unsigned const char*) text);
}

//-----------------------Carré---------------------------
void drawCarre(float x, float y, float z, float lx, float ly, float lz)
{
	glBegin(GL_POLYGON);

	glVertex3f(x, y, z);

	if (lz == 0)
	{
		glVertex3f(x + lx, y, z);
		glVertex3f(x + lx, y + ly, z);
		glVertex3f(x, y + ly, z);
	}
	if (ly == 0)
	{
		glVertex3f(x + lx, y, z);
		glVertex3f(x + lx, y, z + lz);
		glVertex3f(x, y, z + lz);
	}
	if (lx == 0)
	{
		glVertex3f(x, y, z + lz);
		glVertex3f(x, y + ly, z + lz);
		glVertex3f(x, y + ly, z);
	}

	glEnd();
}

float tailleString(char* chaine, void *font){
	return glutBitmapLength(font, (unsigned const char*) chaine);
}

void rectanglePlein(float x1, float y1, float x2, float y2){
	glBegin(GL_QUADS);
		glVertex2f(x1, y1);
		glVertex2f(x2, y1);
		glVertex2f(x2, y2);
		glVertex2f(x1, y2);
	glEnd();
}

void rectangleCreux(float x1, float y1, float x2, float y2){
	glBegin(GL_LINE_STRIP);
			glVertex2f(x1, y1);
			glVertex2f(x1, y2);
			glVertex2f(x2, y2);
			glVertex2f(x2, y1);
			glVertex2f(x1, y1);
		glEnd();
}

void drawCube(float posx, float posy, float posz, float lx, float ly, float lz){
	 glPushMatrix();
	 
	 glTranslatef(posx,posy,posz);
	 float x = lx, y =ly, z = lz;
	 
	//X designe la largeur sur x;
	//Y designe la longueur sur y;
	//Z designe la position sur z;
	// BACK
	glBegin(GL_POLYGON);
	glVertex3f(  x, -y, z );
	glVertex3f(  x,  y, z );
	glVertex3f( -x,  y, z );
	glVertex3f( -x, -y, z );
	glEnd();
	
	// FRONT
	glBegin(GL_POLYGON);
	glVertex3f(  x, -y, -z );
	glVertex3f(  x,  y, -z );
	glVertex3f( -x,  y, -z );
	glVertex3f( -x, -y, -z );
	glEnd();
	 
	// RIGHT
	glBegin(GL_POLYGON);
	glVertex3f( x, -y, -z );
	glVertex3f( x,  y, -z );
	glVertex3f( x,  y,  z );
	glVertex3f( x, -y,  z );
	glEnd();
	 
	// LEFT
	glBegin(GL_POLYGON);
	glVertex3f( -x, -y,  z );
	glVertex3f( -x,  y,  z );
	glVertex3f( -x,  y, -z );
	glVertex3f( -x, -y, -z );
	glEnd();
	 
	// TOP
	glBegin(GL_POLYGON);
	glVertex3f(  x,  y,  z );
	glVertex3f(  x,  y, -z );
	glVertex3f( -x,  y, -z );
	glVertex3f( -x,  y,  z );
	glEnd();
	 
	//  BOTTOM
	glBegin(GL_POLYGON);
	glVertex3f(  x, -y, -z );
	glVertex3f(  x, -y,  z );
	glVertex3f( -x, -y,  z );
	glVertex3f( -x, -y, -z );
	glEnd();
	
	 glPopMatrix();
}

//------------------------USEFUL TOOL------------------

void drawRepere(float unitaire){
	//tracé du repere x,y,z de meme vecteur unitaire
	glColor3f(1,0,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(unitaire,0,0);
	glEnd();
	glColor3f(0,1,0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,unitaire,0);
	glEnd();
	glColor3f(0,0,1);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,0,unitaire);
	glEnd();
}

void drawGrille(int rayon, float pas){
	glPushMatrix();
	glTranslatef(0,-1,0);
	
	glBegin(GL_LINES);
	  for (GLfloat i = -rayon; i <= rayon; i += pas) {
		glVertex3f(i, 0, rayon);
		glVertex3f(i, 0, -rayon);
		glVertex3f(rayon, 0, i);
		glVertex3f(-rayon, 0, i);
	  }
	  glEnd();
	  
	  glPopMatrix();
}


void drawSphere(float posx, float posy, float posz, float rayon, float prec, GLenum style){
	glPushMatrix();
	glTranslatef(posx,posy,posz);
	
	GLUquadric* params = gluNewQuadric();
	gluQuadricTexture(params,GLU_TRUE);
	gluQuadricDrawStyle(params,style);	
	
	gluSphere(params,rayon,prec,prec);
	
	gluDeleteQuadric(params);
	
	glPopMatrix();
}

void draw3D(int fov,float lF,float hF){
	//Affichage 3D
	glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(fov,(double)lF/hF,1,1000);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void draw2D(float largeur, float hauteur){
	//2D- Overlay
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.,(GLdouble)largeur,0.,(GLdouble)hauteur,-1.,1.);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

double pxToCart(double pixel, char axe)
{ //axe = x, y, t (taille)
	if (axe == 'x')
		return (pixel * 20 / (double)1920) - (double)10;
	if (axe == 'y')
		return (pixel * 11.1 / (double)1080) - (double)5.55;
	if (axe == 't')
		return pixel * ((double)20 / (double)1920);

	return 0;
}

double cartToPx(double cart, char axe)
{
	if (axe == 'x')
		return (cart + 10) * (double)1920 / (double)20;
	if (axe == 'y')
		return (cart + 5.55) * (double)1080 / (double)11.1;
	if (axe == 't')
		return cart * ((double)1920 / (double)20);

	return 0;
}

void col(float r, float g, float b, float a){
	glColor4f(r/255.f, g/255.f, b/255.f, a/255.f);
}

float nbToPrct(int num, int num_total){
	return num * 100 / num_total;	
}

void drawPlanete(float rayon, int prec, GLuint texture)
{
    glPushMatrix();

		GLUquadric* params = gluNewQuadric();
			gluQuadricTexture(params,GLU_TRUE);
			
		gluQuadricDrawStyle(params,GLU_FILL);	
		glBindTexture(GL_TEXTURE_2D, texture);
		
		//Dessin de la sphere : couche solide
		glRotatef(84,0,0,1);
		glRotatef(-2,1,0,0);
		glRotatef(6,0,0,1);
		gluSphere(params,rayon,prec,prec);
		
		gluDeleteQuadric(params);


    glPopMatrix();

}
