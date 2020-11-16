#define NB_INFOS 12
#define NB_BUTTONS 17

void interface2d(int menu, int timerAnim, int lF, int hF, Bouton *boutons, char *search, individual *id, Couleur rgb, int panelActive, int *areaSelected, char *value, individual *ptrListe, individual *indiv_to_insert, char tabInfos[NB_INFOS][150]);
void menuPanel(int menu, int timerAnim, int lF, int hF, individual* id, Bouton *boutons, char *search);
int getOld(individual *id);
void findAreaSelected(int panelActive, int x, int y, int lF, int hF, int *areaSelected);
char *textField(char key, char *search, int isNumber);
void insertEditPanel(individual *indiv_to_insert, int **areaSelected, char *value, individual *ptrListe, Bouton *boutons, char tabInfos[NB_INFOS][150], short panel);
void indivEtDeletePanel(individual *id, Bouton *boutons, char tabInfos[NB_INFOS][150], individual *ptrListe, short num);
