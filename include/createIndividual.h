int verifDate(date ref, date test);
int dateNulle(date ref);
individual* cherchePrecedent(individual indiv,individual *ptrTete);
void insereIndiv(individual *indiv, individual **ptrTete, individual *precedent);
individual* rechercheIndiv(char *nom,char *prenom, int gen, individual *ptrTete);
int compareIndiv(individual *src, individual *other);
individual* creerIndiv(indivData data, individual **ptrTete,individual *pere, individual *mere);
void newIndividual(indivData data, individual **ptrTete,individual *pere, individual *mere);
void supprIndiv(individual *ptrSupr,individual **ptrTete);
void afficheIndiv(individual *ptrTete);
int compteIndiv(individual *ptrTete);
void import_indiv (char * nomfichier, individual **ptrtete);
indivData initIndivData(void);
void ecris_indiv(individual *ptrTete,char *nomFichier);
