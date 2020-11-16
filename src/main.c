/**
 * @file main.c
 * @author Thibaud Simon
 * @brief Fichier d'exécution des appels et évenements de l'IHM
 * @version 0.8
 * @date 2019-04-23
 * 
 * @copyright Copyright Thibaud SIMON, Smaniotto Gabriel (c), 2019 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include "../include/aide.h"
#include "../include/chaine.h"
#include "../include/mariage.h"
#include "../include/createIndividual.h"
#include "../include/childLink.h"
#include "../include/weddLink.h"
#include "../include/ia.h"

#include "../include/My_GL.h"
#include "../include/arbre.h"
#include "../include/diagramme.h"
#include "../include/statistique.h"
#include "../include/texture.h"
#include "../include/carto.h"
#include "../include/ville.h"
#include "../include/interface.h"
#include "../include/buttons.h"

/*Déclaration des globales utiles pour le programme */
#define FOV 45
#define FPS 60
#define ANGLE_GAIN 0.25
#define TIMERFIN 120
#define ANIM_SPEED 30
#define RAYON 8

typedef enum btnAction {SEARCH, MODIFIER, DELETE, LINK, INSERT, PERE, MERE, ENFANT, CONJOINT, LOCALISER, APPLY_INSERT, APPLY_EDIT, APPLY_DELETE, PAN_CLOSE, CAPTURE} btnAction;
typedef enum deroulement {VIDE, INTRO, ARBRE, CARTE, STATISTIQUES, DIAGRAMME, OVERLAY}deroulement;
deroulement deroulement3D;
deroulement deroulement2D;

Camera Vision;
Souris S;
GLuint tex_planete;

int timerAnim=-1;
int timerCamIndiv =-1;
int init_time, final_time, frame_count;
int lF, hF;
int gen_chose, indiv_chose_num;
int searchfieldSelected;
int rotate;
int shadow;
int menu;
int *tabGen;
int nb_indiv;
int genmax;
int panelActive;
int areaSelected;

float y_cercle;

char fps_buffer[12];
char search[50];
char *lastCity;
char value[100];
char tabInfos[NB_INFOS][150] = {"Surname : ", "Name : ", "Age : ", "Birth date : ", "Birth location : ", "Death date : ",
	"Death location : ", "Gender : ", "Generation : ", "Father : ", "Mother : ", "Child : "};
char infos_base[NB_INFOS][150] = {"Surname : ", "Name : ", "Age : ", "Birth date : ", "Birth location : ", "Death date : ",
	"Death location : ", "Gender : ", "Generation : ", "Father : ", "Mother : ", "Child : "};

individual *ptrListe = NULL;
wedding *ptrMariage = NULL;

individual *id;
individual indiv_to_insert;
Couleur rgb;
float rotx, rotz;

OBJ3D cam = {0,0,0};
OBJ3D lastCamCenter;
OBJ3D pinedCity;
Indiv3D indiv_chosen;
Indiv3D **indiv_coords;

Bouton boutons[NB_BUTTONS];

//Prototypes
void init(void);
void display(void);
void update(int g);

void GestionClavierSpecial(int key, int x, int y);
void GestionClavier(unsigned char key, int x, int y);

void updateFPS();
void GereSouris(int button,int state,int x, int y);
void DeplacementSouris(int x, int y);
void DeplacementPassifSouris(int x, int y);
void ResizeWindow(int width, int height);

void btnPere(void);
void btnMere(void);
void btnEnfant(void);
void btnConjoint(void);
void randomizeColor(Couleur *rgb);
void initTree(individual *ptrListe);
void actionIndiv(void);

/* Actions boutons : à déplacer dès que possible */

void btnSaveCSV(void){
	printf("Sauvegarde du fichier individus... : ../ressources/IndivSauvegarde.csv\n");
	ecris_indiv(ptrListe , "../ressources/IndivSauvegarde.csv");
	
	printf("Sauvegarde du fichier mariages... : ../ressources/mariagesSauvegarde.csv\n");
	ecris_fichier_mariage("../ressources/mariagesSauvegarde.csv", ptrMariage);
	
	printf("Fichiers sauvegardés\n");
}

void btnProcessIA(void){
		//AI
	find_lieu_naissance(ptrListe);
	find_date_naissance(ptrListe);
	find_lieu_mort(ptrListe);
	find_lieu_mariage(ptrMariage);
	find_date_mariage(ptrMariage);
	find_date_mort(ptrListe);
}

void btnpanClose(void){
	panelActive = -1;
}

individual* btnSearch(char *search, individual *ptrTete){
	char c = search[0]; int index = 0; int hasSpace = 0;
	int indexSpace = 0;
	individual *idd = NULL;
	
	printf(" research Indiv = '%s'\n", search);
    printf("search button toggled!\n");
    
	
	while( c != '\0'){
		if(c == ' '){ //si egal au caractere ASCII espace
			hasSpace++;
			printf("espace détécté à emplacement %d\n",index);
			indexSpace = index;
		}
		index++;
		c = search[index];
	}
	
	if(hasSpace == 1){
		printf("La chaine possède un espace\n");
		char* nom = search+indexSpace+1;
		char prenom[50];
		strcpy(prenom, search);
		prenom[indexSpace] = '\0';
		
		printf("1er mot : '%s'\n",nom);
		printf("2eme mot : '%s'\n",prenom);
		
		//On recherche pour chaque gen
		int gen = 1;
		while(gen <= genmax){
			printf("gen = %d\n",gen);
			idd = rechercheIndiv( nom, prenom, gen, ptrTete);
			if(idd !=NULL){
				gen = genmax+1;
			}
			else{
				gen++;
			}
		}
		
		if(idd == NULL){ //Recherche infructueuse : on essaie d'inverser les champs
			gen = 1;
			while(gen <= genmax){
				printf("gen = %d\n",gen);
				idd = rechercheIndiv( prenom, nom, gen, ptrTete);
				if(idd !=NULL){
					gen = genmax+1;
				}
				else{
					gen++;
				}
			}
		}
		
		if(idd != NULL){
			printf("id n'est plus NULL\n");
			return idd;
		}
		else{
			printf("No individual found ! \n");
		}
	}
	else if(hasSpace == 0){
		printf("pas d'espace !\n");
		//Pas d'espace, on cherche un nom unique
		individual *ptr= ptrTete ;
		
		while(ptr != NULL && idd==NULL){
			if(strcasecmp(search, ptr->infos.name) == 0)
				idd = ptr;
			else if(strcasecmp(search, ptr->infos.surname) == 0)
				idd = ptr;
			ptr = ptr->next;
		}
		if(idd !=NULL && strcmp(idd->infos.name,"")){
			printf("id n'est plus NULL\n");
			printf("individu identifié : %s %s\n",idd->infos.name, idd->infos.surname);
			return idd;
		}
		else{
			printf("No individual found ! \n");
		}
	}
	return NULL;

}

void btnLink(void){
    printf("link button toggled!\n");
}

void btnInsert(int *panelActive, char tabInfos[NB_INFOS][150]){
	*panelActive = 1;
	printf("panel Active set to 1\n");
	id = NULL;
    for(int i=0; i<NB_INFOS; i++){
		strcpy(tabInfos[i],infos_base[i]);
		if(i != 2)
		strcat(tabInfos[i], " Click to edit");
	}
}

void btnModifier(int *panelActive, char tabInfos[NB_INFOS][150]){
	*panelActive = 2;
	printf("panel Active set to 2\n");
	
	for(int i=0; i<NB_INFOS; i++){
		strcpy(tabInfos[i],infos_base[i]);
		if(i != 2)
		strcat(tabInfos[i], " Click to edit");
	}
    
}

void btnDelete(individual indiv_to_insert, int *panelActive){
	*panelActive = 3;
	printf("panel Active set to 3\n");
	
	for(int i=0; i<NB_INFOS; i++){
		strcpy(tabInfos[i],infos_base[i]);
		strcat(tabInfos[i], "Unknown");
	}
}

void btnApplyDelete(individual *id, individual *ptrListe, int *panelActive){
	if(id != NULL){
		gen_chose = id->infos.num_gen-1;
		tabGen[gen_chose] -= 1;
		supprIndiv(id, &ptrListe);
		initTree(ptrListe);
		id = NULL;
	}
	*panelActive = 0;
}

bool isValid(individual indiv){
	if( strcmp(indiv_to_insert.infos.name,"") 
	&& strcmp(indiv_to_insert.infos.surname,"") 
	&& (indiv.infos.gender=='M' || indiv.infos.gender=='F')
	&& (indiv.infos.num_gen> 0 && indiv.infos.num_gen <= genmax)
	){
		//On verifie que l'individu n'existe pas encore
		if( rechercheIndiv(indiv.infos.surname, indiv.infos.name, indiv.infos.num_gen, ptrListe) == NULL ) {
			return true;
		}
	}
	
	return false;
}

void btnApplyInsert(individual indiv_to_insert, int *panelActive){
	if(isValid(indiv_to_insert)){
		*panelActive = 0;
		individual *pere = rechercheIndiv(indiv_to_insert.father->infos.surname, indiv_to_insert.father->infos.name, indiv_to_insert.infos.num_gen+1, ptrListe);
		individual *mere = rechercheIndiv(indiv_to_insert.mother->infos.surname, indiv_to_insert.mother->infos.name, indiv_to_insert.infos.num_gen+1, ptrListe);
		newIndividual(indiv_to_insert.infos, &ptrListe, pere, mere);
		
		*panelActive = -1;
		id = rechercheIndiv(indiv_to_insert.infos.surname, indiv_to_insert.infos.name, indiv_to_insert.infos.num_gen, ptrListe);
		
		//On place le curseur sur l'individu
		initTree(ptrListe);
		actionIndiv();
	}
}

void btnApplyEdit(int *panelActive){
	*panelActive = 0;
}

/**
 * @brief appel de la boucle d'événements et affectation d'ordre général.
 * 
 */
int main(int argc, char *argv[])
{
    glutInit(& argc, argv );
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    
	//Proprietes definies pour la fenetre
    glutInitWindowSize(LARGEUR_FENETRE,HAUTEUR_FENETRE);
    glutInitWindowPosition(100,100);
    
    //Creation de la fenetre
        glutCreateWindow("SGT | Smart Genealogic Tree");
    
    //Fonction d'initialisation
	init();
	
	//Fonction de callback temporisee (effectuee apres 1000 ms) appelée 1 fois
	glutTimerFunc(1000/FPS, update, 0);
	
	//Fonction des Inputs utilisateur
		//Souris (boutons)
		glutMouseFunc(GereSouris);
		
		//Souris (mouvement appuyé)
		glutMotionFunc(DeplacementSouris);
		
		//Souris  (mouvement passif)
		glutPassiveMotionFunc(DeplacementPassifSouris);
	
		//Clavier
		glutKeyboardFunc(GestionClavier);
		
		//Touches media / autres que des lettres:
		glutSpecialFunc(GestionClavierSpecial);
	
	//Fonction d'affichage
	glutDisplayFunc(display);

	//Fonction redimensionnement
	glutReshapeFunc(ResizeWindow);
	
	//Loop
	glutMainLoop();

    return 0;
}

/**
 * @brief initialisation(constructeur) en une ligne de boutons par affectations de paramètres
 * 
 * @param b Bouton en entrée
 * @param x1 coordonnée d'abscisse en entrée
 * @param y1 coordonée d'ordonnée en entrée
 * @param x2 coordonnée d'abscisse en sortie
 * @param y2 coordonnée d'ordonée en sortie
 * @param label chaine(char*) à afficher dans le bouton
 * @param action fonction (void*) à executer sur clic
 * @param r valeur de rouge
 * @param g valeur de vert
 * @param B valeur de bleu
 */
void initBtn( Bouton *b, float x1, float y1, float x2, float y2, char* label, ButtonCallback action, int r, int g, int B){
	b->x1 = x1;
	b->x2 = x2;
	b->y1 = y1;
	b->y2 = y2;
	
	b->clicked=0;
	b->hovered=0;
	b->color.r = r;
	b->color.g = g;
	b->color.b = B;
	
	b->label = (char *) malloc(sizeof(char*)*strlen(label));
	strcpy(b->label, (char*)label);
	b->action = action;
}

void chooseIndividual(Couleur* col, Indiv3D* indiv_3D, individual** individu){
	*indiv_3D= indiv_coords[gen_chose][indiv_chose_num];
	if( *individu != NULL)
		lastCity = (*individu)->infos.birth.location;
	else{
		lastCity = malloc(sizeof(char)*2);
		strcpy(lastCity,"");
	}
	
	*individu = rechercheIndiv(indiv_chosen.nom, indiv_chosen.prenom, gen_chose+1, ptrListe);
	if( *individu != NULL){
		randomizeColor(col);
	}
	if(deroulement3D == CARTE){
		btnLocaliser(id, &pinedCity, lastCity);
	}
	panelActive = 0;
}

/**
 * @brief initialisation des différents boutons du programme
 * @param boutons tableau des boutons (Bouton*)
 */
void initBtns(Bouton *boutons){
	float pan_startx = 2*lF/3+ lF/30;
	float pan_endx = lF - lF/30;
	
	//Panneau latéral
	initBtn(&(boutons[0]), pan_startx, 100, pan_endx, 150, "Recherche", NULL, 53,166,155);
	initBtn(&(boutons[1]), pan_startx, 300, pan_endx, 340, "Modifier Individu", NULL, 53,166,155);
	initBtn(&(boutons[2]), pan_startx, 360, pan_endx, 400, "Supprimer Individu", NULL, 53,166,155);
	initBtn(&(boutons[3]), pan_startx, 420, pan_endx, 460, "Modification de lien", NULL, 53,166,155);
    initBtn(&(boutons[4]), pan_startx, 480, pan_endx, 520, "Inserer Individu", NULL, 53,166,155);
    
    //Panneau individu
    initBtn(&(boutons[5]), lF/35, hF/6, lF/35 + 100, hF/4, "Pere", btnPere, 53,166,155);
    initBtn(&(boutons[6]), lF/3.6-40,hF/6, lF/3.6 + 60, hF/4, "Mere", btnMere, 53,166,155);
    
    initBtn(&(boutons[7]), lF/35, hF*33/40, lF/35 + 100, hF*9/10, "1er Enfant", btnEnfant, 53,166,155);
    initBtn(&(boutons[8]), lF/3.6-40, hF*33/40, lF/3.6 + 60, hF*9/10, "Conjoint", btnConjoint, 53,166,155);
    initBtn(&(boutons[9]), lF/35+105, hF*33/40,lF/3.6-45, hF*9/10, "Localiser", btnLocaliser, 53,166,155);
    
    initBtn(&(boutons[10]), lF/35+105, hF*33/40,lF/3.6-45, hF*9/10, "Inserer", NULL, 53,166,155);
    initBtn(&(boutons[11]), lF/35+105, hF*33/40,lF/3.6-45, hF*9/10, "Appliquer", NULL, 53,166,155);
    initBtn(&(boutons[12]), lF/35+105, hF*33/40,lF/3.6-45, hF*9/10, "Supprimer", NULL, 53,166,155);
    
    initBtn(&(boutons[13]), lF/3.6 + 40,hF/6 - 45, lF/3.6 + 90, hF/6 - 20, "X", btnpanClose, 188, 41, 53);
    initBtn(&(boutons[14]), pan_endx - 90, 10, pan_endx - 10, hF/15, "Capture", btnScreenshot, 53,166,155);
    
    initBtn(&(boutons[15]), pan_startx, 540, pan_endx, 580, "Process AI", btnProcessIA, 203, 158, 99);
    initBtn(&(boutons[16]), pan_startx, 600, pan_endx, 640, "Sauver vers csv", btnSaveCSV, 53,166,155);
}

/**
 * @brief initialisation en une ligne de l'objet caméra, par affectation sur pointeurs
 * 
 * @param Vis Structure de type Camera*
 * @param eyeX axe x de l'oeil utilisateur
 * @param eyeY axe y de l'oeil utilisateur
 * @param eyeZ axe z de l'oeil utilisateur
 * @param cX centrage x de la caméra
 * @param cY centrage y de la caméra
 * @param cZ centrage z de la caméra
 * @param upX vecteur (x)
 * @param upY vecteur (y)
 * @param upZ vecteur (z)
 */
void initCam( Camera* Vis, float eyeX, float eyeY, float eyeZ, float cX, float cY, float cZ, float upX, float upY, float upZ){
	Vis->eye.x = eyeX;
    Vis->eye.y= eyeY;
    Vis->eye.z = eyeZ;
    Vis->center.x = cX;
    Vis->center.y = cY;
    Vis->center.z = cZ;
    Vis->up.x = upX;
    Vis->up.y = upY;
    Vis->up.z = upZ;
}

void initTree(individual *ptrListe){
	//Initialisation of max_indiv
	individual *ptr = ptrListe;
	int max = 0;
	while(ptr != NULL){
		if(max < ptr->infos.num_gen) max = ptr->infos.num_gen;

		ptr = ptr->next;
	}
	
	//Initialisation of tabGen
	tabGen = malloc(max* sizeof(int));
	tabGen = (int *) malloc(max* sizeof(int));
	for(int i=0; i < max; i++){
		tabGen[i] = compte_indiv(i+1, ptrListe);
	}
	genmax= max;
	
	
	indiv_coords = createArbre(RAYON, ptrListe, genmax, nb_indiv, tabGen);
	deroulement3D = ARBRE;
}

/**
 * @brief Fonction init : 
 *   initialisation des variables existantes,
 *   appel à des fonctions d'affichage OpenGL
 */
void init(void){
	printf("INITIALISATION IN PROGRESS\n\n");

	tex_planete = loadTexture("../ressources/5k.jpeg",false);
	
	//Importing individuals
	import_indiv("../ressources/liste_Individus.csv",&ptrListe);
	nb_indiv = compteIndiv(ptrListe);
	printf("succeeded at importing individuals ✔️\n");
	
	//Import wedding
	import_wedding("../ressources/mariage.csv",&ptrListe,&ptrMariage);
	printf("succeeded at importing wedding ✔️\n");
	
	//Associating childrens to fathers
	childLinkerFath(ptrListe);
	printf("succeeded at linking childrens to their father ✔️\n");
	
	//Associating childrens to mothers
	childLinkerMoth(ptrListe);
	printf("succeeded at linking childrens to their mother ✔️\n");
	
	//Associating husbands
	createWeddLinkHusband(ptrMariage);
	printf("succeeded at associating wedding with husbands ✔️\n");
	
	//Associating wives
	createWeddLinkWife(ptrMariage);
	printf("succeeded at associating wedding with wives ✔️\n");

	//Clear color window
	glClearColor(0.95, 0.95, 0.95, 1.0);

	hF = HAUTEUR_FENETRE;
	lF = LARGEUR_FENETRE;
	
	//Var initialisations
	indiv_chose_num = 0;
	gen_chose = 0;
	y_cercle = 5;
	init_time = 0;
	final_time = 0;
	frame_count = 0;
	rotate = 1;
	shadow = 0;
	menu = 0;
	panelActive = -1;
	areaSelected = 0;
	indiv_to_insert.infos = initIndivData();
	
	deroulement3D = VIDE;
	deroulement2D = OVERLAY;
	
	lastCity = malloc(sizeof(char)*2);
	strcpy(lastCity,"");
	
	//Init random seed
	srand(time(NULL));
	
    //Initialisation de la Camera
    initCam(&Vision,0,0,20, 0,0,0, 0,1,1);
	
	//Initialisation des boutons
	initBtns(boutons);

    //Initialisation à 0 des variables globales
    init_time = time(NULL),
	angle_camera = 0;
	searchfieldSelected = 0;
	
	//Generation de l'arbre
	initTree(ptrListe);

	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_FOG);
    {
        GLfloat fogColor[4] = {0.95, 0.95, 0.95, 1.0 };
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_DENSITY, 0.4);
        glFogf(GL_FOG_START, 15.0);
        glFogf(GL_FOG_END, 30.0);
    }
	glDisable(GL_FOG);
	
	// Appel des actifs openGL
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); /* Correction de perspective */
   	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); /* Type de test de profondeur */

	glShadeModel(GL_SMOOTH); // Rendu lisse

	/* Mise en place de l'anti-aliasing */
	//Transparence
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//Lissage
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	//Règles de lissage
	glHint (GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//Colorimetrie
	glEnable(GL_COLOR_MATERIAL);
	/* Normalisation des vecteurs */
	glEnable(GL_NORMALIZE);
 }

/**
 * @brief Fonction d'affichage
 * Dessin de l'IHM
 * 
 */
void display(void){
	//Fonction d'affichage
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//RAFRAICHIS FENETRE

	//////////////////////////////// PARTIE 3D /////////////////////////
		/* Dessin 3D */
		draw3D(FOV, lF, hF);
		glEnable(GL_COLOR_MATERIAL);

		//Distance : On place la camera
		gluLookAt(Vision.eye.x, Vision.eye.y, Vision.eye.z, Vision.center.x,
		Vision.center.y, Vision.center.z, Vision.up.x, Vision.up.y, Vision.up.z);

		glTranslatef(0,0, cam.z);
		glRotatef(angle_camera, 0, 1, 0);
		if (shadow)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_FOG);
		}

		switch(deroulement3D){
			
			case INTRO:
			//Affichage de la sphère d'intro
			col(50, 20, 150, 100);
			if (y_cercle < 20)
			{
				col((int)(y_cercle)*10,(int)(y_cercle)*10,(int)(y_cercle)*10,100);
				glTranslatef(0,0,-20);
				rectanglePlein(-lF,-hF,lF,hF);
				glTranslatef(0,0,20);
				col(80,180,200,255);
				glRotatef(y_cercle*5, 1, 1, 1);
				drawSphere(0,0,0,RAYON,y_cercle,LIGNES);
				glRotatef(-y_cercle*5, 1, 1, 1);
			}
			break; //FIN
			
			case ARBRE:
			//Affichage des points de l'arbre
			for(int i=0; i< genmax; i++){
				for(int j = 0; j < tabGen[i]; j++){
					float calc = 1.0-((float)i/(float)genmax);
					glColor3f(calc*0.35, calc*0.6, calc*0.7);
					OBJ3D obj = indiv_coords[i][j].coords;
					drawSphere(obj.x, obj.y, obj.z, 0.3/(i+1), 10, 0);
				}
			}
			
			//Affichage de l'individu en selection dans l'arbre
			if(id != NULL){
				traceLiensIndividu(id, indiv_chosen, tabGen, indiv_coords, gen_chose);
				traceLiensMariages(id, indiv_chosen, tabGen, indiv_coords, gen_chose);
				col(255, 255, 0, 100);
				drawSphere(indiv_chosen.coords.x, indiv_chosen.coords.y, indiv_chosen.coords.z, 0.3, 10, 0);
			}
			break; //FIN
			
			case CARTE:
			//Affichage de la map monde
			displayCarte(&Vision, pinedCity, id->infos.birth.location, tex_planete,rotx,rotz);
			break; //FIN

			default:
			break;
		}
	//////////////////////////////// PARTIE 2D /////////////////////////
	draw2D(lF, hF);
	//Desactivation des ombrages, couleurs
	glDisable(GL_COLOR_MATERIAL);
	if (shadow) glDisable(GL_LIGHTING);
	
	switch(deroulement2D){
		
		case STATISTIQUES:
		//Affichage des statistiques
		//statistique(ptrListe, tabIndiv.nb_indiv_tot, "old");
		break;
		
		case DIAGRAMME:
		//Affichage du diagramme
		//diagramme(ptrListe, tabIndiv.nb_indiv_tot);
		break;

		case OVERLAY:
		//Affichage de l'interface
		if(deroulement3D > INTRO){
			interface2d(menu, timerAnim, lF, hF, boutons, search, id, rgb, panelActive, &areaSelected, value, ptrListe, &indiv_to_insert, tabInfos);
		}
		break;
		
		default:
		break;
	}
	//Affichage du compteur d'IPS
	glColor3f(0.5,0.5,0.5);
	afficheString(GLUT_BITMAP_HELVETICA_12,fps_buffer,5,hF - 16);
		
	//Nettoyage
	glutSwapBuffers();
	updateFPS();
}

void update(int g){
	/* Gestion des timers */
	//Animation ouverture panneau (2D)
	if(timerAnim != -1 && timerAnim < lF/3)
			timerAnim+= ANIM_SPEED;
	if(timerAnim >= lF/3) timerAnim = -1;

	//Animation transition camera entre individus (3D)
	if(timerCamIndiv != -1 && timerCamIndiv < 20)
			timerCamIndiv += ANIM_SPEED;
	if(timerCamIndiv >= 20) timerCamIndiv = -1;
	
	//Animation rotation sphere au lancement
	if (y_cercle < 20 && deroulement3D==INTRO){
		y_cercle += 0.2;
	}
	else if(y_cercle < 21){
		rotate = 1;
		deroulement3D = ARBRE;
		y_cercle = 23;
	}
	
	//Reaffichage demandé
	glutPostRedisplay();
	
	//Mouvance angle camera
	if(rotate != 0) angle_camera+= ANGLE_GAIN;
	if(angle_camera == 360) angle_camera = 0;
	
	//Rappeler cette fonction dans 1/60 secondes
	glutTimerFunc(1000/FPS, update, 0);
}

/**
 * @brief met à jour le compteur des Images Par Secondes (Frames Per Second)
 * 
 */
void updateFPS(void){
	frame_count++;
	final_time = time(NULL);
	if(final_time - init_time > 1)
	{
		sprintf(fps_buffer,"FPS : %d\n",frame_count/(final_time-init_time));
		frame_count =0;
		init_time = final_time;
	}
}


void GereSouris(int button,int state,int x, int y){
	if (state == GLUT_DOWN) 
	{ //Bouton pressé
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			for(int i=0; i< NB_BUTTONS; i++){
				if(insideBtn(boutons[i],x,y)){
					boutons[i].clicked = boutons[i].clicked?0:1; //Verification: si b est deja clicked(1), clicked =0, sinon on passe à 1
				}
			}
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		}
	}
	else 
	{ //Bouton relaché
		switch(button) 
		{
		case GLUT_LEFT_BUTTON:
			for(btnAction i=0; i< NB_BUTTONS; i++){
				if(insideBtn(boutons[i],x,y)){
					boutons[i].clicked = boutons[i].clicked?0:1; //Verification: si b est deja clicked(1), clicked =0, sinon on passe à 1
					switch(i){
						case LOCALISER:
						if(panelActive < 1){
							if(deroulement3D == CARTE) deroulement3D = ARBRE;
							else{
								deroulement3D = CARTE;
								btnLocaliser(id, &pinedCity, lastCity);
							}
						}
						break;
						
						case DELETE:
						if(id != NULL)
							btnDelete(indiv_to_insert, &panelActive);
						break;
						
						case MODIFIER:
						if(id != NULL)
							btnModifier(&panelActive, tabInfos);
						break;
						
						case LINK:
						btnLink(); //TODO
						break;
						
						case SEARCH:
						if(strcmp(search,"")){
							id = btnSearch(search, ptrListe);
							//Affectation indiv_chosen, localisation
							//actionIndiv();
							panelActive = 0;
						}
						break;
						
						case INSERT:
						btnInsert(&panelActive, tabInfos);
						break;
						
						case APPLY_INSERT:
						if(panelActive == 1)
							btnApplyInsert(indiv_to_insert, &panelActive);
						break;

						case APPLY_EDIT:
						if(panelActive == 2)
							btnApplyEdit(&panelActive);
						break;
						
						case APPLY_DELETE:
						if(panelActive == 3)
							btnApplyDelete(id, ptrListe, &panelActive);
						break;
						
						case CAPTURE:
						if(!menu)
						btnScreenshot(lF, hF);
						break;

						default:
						boutons[i].action();
						break;
					}
				}
			}
			S.x = x;
			S.y = y;
			findAreaSelected(panelActive, S.x, S.y, lF, hF, &areaSelected);
			break;
			
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		}
		
	}
	
	//button 3 = scroll up
	if(button == 3){
		if(!menu && !areaSelected){
				cam.z += 0.3;
			}
	}
	
	//button 4 = scroll down
	if(button == 4){
		if(!menu && !areaSelected){
				cam.z -= 0.3;
			}
	}
	
}

void DeplacementSouris(int x, int y){
	float progx, progy;
	int oldx, oldy;
	oldx = S.x;
	oldy = S.y;
	
	S.x = x;
	S.y = y;
	
	progx = x - oldx;
	progy = y - oldy;
	
	if(abs(progx) >= abs(progy))
		rotx += progx;
	else
		rotz += progy;
}

void DeplacementPassifSouris(int x, int y){
	for(int i=0; i< NB_BUTTONS; i++){
		boutons[i].hovered = insideBtn(boutons[i],x,y)?1:0; //Verification : hovered = 1 si inside =1, hovered = 0 si inside =0
	}
}

void GestionClavierSpecial(int key, int x, int y){
	switch (key){
		case GLUT_KEY_LEFT:
		if(panelActive <= 0){
			if(indiv_chose_num == tabGen[gen_chose]){
				indiv_chose_num = 0;
			}
			else{
				indiv_chose_num--;
			}
			chooseIndividual(&rgb, &indiv_chosen, &id);
		}
		break;


		case GLUT_KEY_RIGHT:
		if(panelActive <= 0){
			if(indiv_chose_num == tabGen[gen_chose]){
				indiv_chose_num = 0;
			}
			else{
				indiv_chose_num++;
			}
			chooseIndividual(&rgb, &indiv_chosen, &id);
		}
		break;


		case GLUT_KEY_UP:
		if(panelActive <= 0){
			indiv_chose_num = 0;
			if(gen_chose == genmax-1){
				gen_chose = 0;
			}
			else{
				gen_chose++;
			}
			chooseIndividual(&rgb,&indiv_chosen,&id);
		}
			break;


		case GLUT_KEY_DOWN:
			if(panelActive <= 0){
				indiv_chose_num = 1;
				if(gen_chose == 0){
					gen_chose = genmax-1;
				}
				else{
					gen_chose--;
				}
				chooseIndividual(&rgb,&indiv_chosen,&id);
			}
			break;
			
	}
}

void GestionClavier(unsigned char key, int x, int y){
	switch(key){
		
		case 27: //ESC
			exit(0);
			break;
		
		case 'r':
			if(!menu && !areaSelected){
				if(rotate == 0) rotate = 1;
				else rotate = 0;
			}
			break;
		
		case 's':
			if(!menu && !areaSelected){
				cam.z -= 0.2;
			}
			break;
		
		case 'z':
			if(!menu && !areaSelected){
				cam.z += 0.2;
			}
			break;
			
		
		case 'l':
		if(!menu && !areaSelected){
				if (shadow == 1){
					shadow = 0;
					glDisable(GL_FOG);
				}
				else{
					shadow = 1;
				}
			}
		break;

		//Appui sur tab
		case 9:
			if(menu==1){
				menu=0;
				searchfieldSelected=0;
			}
			else{
				 menu =1;
				 searchfieldSelected=1;
			}
			
			timerAnim =0;
		break;

	}

	//Entrée dans le champ de recherche
	if(deroulement2D==OVERLAY && menu && searchfieldSelected && timerAnim ==-1){
		textField(key, search, false);
	}
	if(panelActive && areaSelected){
		if(menu == 1){ 
			menu=0;
			searchfieldSelected=0;
			timerAnim = 0;
		}
		if(areaSelected == 3 || areaSelected == 4 || areaSelected == 6 || areaSelected == 9){
			//Nombres
			strcpy(value, textField(key, value, 1));
		}
		else{
			//Lettres
			strcpy(value, textField(key, value, 0));
		}
	}
		
}

void ResizeWindow(int width, int height) {
	int w = width; int h = height;
	//Eviter les soucis liés à une division par 0
	if(h == 0)
		h = 1;
	float ratio = w/h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w,h);

	// Set the correct perspective.
	gluPerspective(FOV,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
	
	lF = w;
	hF = h;
	
	initBtns(boutons);
}

//Actions globales..

void randomizeColor(Couleur *rgb){
	rgb->r = rand()%255;
	rgb->g = rand()%255;
	rgb->b = rand()%255;
}

void actionIndiv(void){
	randomizeColor(&rgb);
	
	indiv_chosen = *getIndiv3DfromIndivListe( id, indiv_coords, tabGen, gen_chose);
	
	if(deroulement3D == CARTE){
		indiv_chosen = indiv_coords[gen_chose][indiv_chose_num];
		if( id != NULL)
			lastCity = id->infos.birth.location;
		else{
			lastCity = malloc(sizeof(char)*2);
			strcpy(lastCity,"");
		}
		btnLocaliser(id, &pinedCity, lastCity);
	}
	
}

void btnPere(void){
	printf("bouton pere appuyé\n");
	if(id->father != NULL && gen_chose < genmax){
		id = id->father;
		gen_chose++;
		actionIndiv();
	}
	
}

void btnMere(void){
	printf("bouton mere appuyé\n");
	if(id->mother != NULL && gen_chose < genmax){
		id = id->mother;
		gen_chose++;
		actionIndiv();
	}
	
}

void btnEnfant(void){
	printf("bouton enfant appuyé\n");
	if(id->firstChild != NULL){
		id = id->firstChild->child;
		randomizeColor(&rgb);
		gen_chose -= 1;
		
		actionIndiv();
	}
	
}

void btnConjoint(void){
	printf("bouton conjoint appuyé\n");
	if(id->firstUnion != NULL){
		if(id->infos.gender == 'M')
			id = id->firstUnion->mariage->info_wedding.wife;
		else
			id = id->firstUnion->mariage->info_wedding.husband;
		randomizeColor(&rgb);
		gen_chose = id->infos.num_gen-1;
		
		actionIndiv();
	}
	
}
