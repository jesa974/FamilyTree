// AFFICHAGE DES VALEURS DE LA LISTE MARIAGE
void display_values (wedding* ptr_tete);

// CREATION D'UN MARIAGE
wedding *create_wedding (weddData value);

// RECHERCHE DU DERNIER MARIAGE
wedding* find_last_wedding(wedding* ptr_tete);

// INSERTION D'UN MARIAGE
void insert_wedding (wedding** ptr_tete, wedding* new);

// INSERTION D'UNE VALEUR DANS LA LISTE
void insert_value (wedding** ptr_tete, weddData value);

// CREATION D'UNE LISTE SANS SAISIE DE VALEURS
void create_list_wedd (wedding **ptrTete, weddData data);

// COMPTEUR NOMBRE DE MARIAGE
int cpt_wedding(wedding *ptrTete);

// ECRITURE FICHIER MARIAGE
void ecris_fichier_mariage(char *nomSortie, wedding *ptrTete);

void import_wedding(char * nomfichier, individual **ptrtete, wedding **ptrTete);
