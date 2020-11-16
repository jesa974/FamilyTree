
int compteMariage(weddLink *ptrTete);

int compteEnfants(childLink *ptrTete, individual *indiv);

char **findTabVilleEnfants(childLink *ptrTete, char **tabVille);

char *findBirthLocationWedding(individual *person, weddLink *ptrTete);

void findBirthDateWedding(individual *person, weddLink *ptrTete);

char *findBirthLocationChild(int nbFreres, childLink *listeEnfants);

void find_lieu_naissance(individual *ptrTete);

void find_date_naissance(individual *ptrTete);

int testMort_avecDate(individual *ptr);

void find_lieu_mort(individual *ptrTete);

void find_lieu_mariage(wedding *ptrTete);

void find_date_mariage(wedding *ptrTete);

void find_date_mort(individual *ptrTete);
