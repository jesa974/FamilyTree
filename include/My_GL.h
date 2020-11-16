//DEFINITIONS

#define PLEIN GLU_FILL
#define SILHOUETTE GLU_SILHOUETTE
#define LIGNES GLU_LINE
#define POINTS GLU_POINT
#define PI 3.14159
#define X 'x'
#define Y 'y'
#define Z 'z'
#define T 't'


#define LARGEUR_FENETRE 1280
#define HAUTEUR_FENETRE 720

extern int hF;
extern int lF;

//Variables globales
double angle_camera;

//Definitions de structures
	
typedef struct Couleur{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Couleur;

/*Structure bouton: un etat, des coordonnees et une action*/
typedef void (*ButtonCallback)();
typedef struct Bouton{
	float x1, y1; //Coordonées 1=debut
	float x2, y2; //Coordonées 2=fin
	Couleur color;
	int clicked;	  //Etat : appuyé ou non (1:0)
	int hovered;  //Etat : survolé ou non (1:0)
	char* label; //Texte à afficher
	ButtonCallback action; //Fonction à executer
}Bouton;

/*Structure Souris : utile pour instance et verification de l'état et de la position du curseur */
typedef struct Souris{
	float x;
	float y;
	int pressed;
	int mouvance;
	float px;
	float py; //Coordonnées passives
}Souris;

/*Structure de coordonnees */
typedef struct OBJ3D{
	float x;
 	float y;
 	float z;
 }OBJ3D;

 typedef struct Indiv3D{
	 OBJ3D coords;
	 char nom[50];
	 char prenom[50];
 }Indiv3D;

/*Structure Camera : Utile pour définir la fonction LookAt() */
typedef struct Camera{
	OBJ3D eye;
 	OBJ3D center;
 	OBJ3D up;
}Camera;

//Fonctions de dessin 2D
void drawCercle(float rIn, float rOut, float x, float y, int segments);
void drawDisque(float r, float x, float y, int precision, int decoupage);
void drawCarre(float x, float y, float z, float lx, float ly, float lz);
void afficheString(void * font, char *text, int x, int y);
float tailleString(char* chaine, void *font);
void rectanglePlein(float x1, float y1, float x2, float y2);
void rectangleCreux(float x1, float y1, float x2, float y2);
void drawDiagramme(float r, float x, float y, float z, char axe1, char axe2, int sommets, int num1, int nb_indiv);

//Fonctions de dessin 3D
void drawCube(float posx, float posy, float posz, float lx, float ly, float lz);
void drawSphere(float posx, float posy, float posz, float rayon, float prec, GLenum style);
void drawGrille(int rayon, float pas);
void drawRepere(float unitaire);
void afficheLigne(float x1, float y1, float z1, float x2, float y2, float z2);

//Fonctions de repères
void draw3D(int fov, float lF, float hF);
void draw2D(float largeur, float hauteur);

//Boutons
int isIn(float x1b, float x2b, float y1b, float y2b, float x, float y);
int insideBtn(Bouton b, int x, int y);
void afficheBtn(Bouton *btn);
void afficheBtnGrise(Bouton *btn);

//Fonctions utiles
float frand(float a, float b);
double pxToCart(double pixel, char axe);
double cartToPx(double cart, char axe);
void col(float r, float g, float b, float a);
float nbToPrct(int num, int num_total);

//IHM

void drawPlanete(float rayon, int prec, GLuint texture);


/* Polices d'écritures Bitmap */
	//Serif
	#define TIMES_10 GLUT_BITMAP_TIMES_ROMAN_10
	#define TIMES_24 GLUT_BITMAP_TIMES_ROMAN_24

	//Sans serif
	#define SANS_SERIF_8 GLUT_BITMAP_8_BY_13
	#define SANS_SERIF_9 GLUT_BITMAP_9_BY_15

	#define HELVETICA_10 GLUT_BITMAP_HELVETICA_10
	#define HELVETICA_12 GLUT_BITMAP_HELVETICA_12
	#define HELVETICA_18 GLUT_BITMAP_HELVETICA_18
	
