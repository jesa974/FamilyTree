int compte_indiv(int gen, individual* ptrTete);
Indiv3D** createArbre(float rayon, individual *ptrTete, int maxgen, int nb_indiv, int *nb_per_gen);
Indiv3D* getIndiv3DfromIndivListe(individual* indiv, Indiv3D** tableau, int* tabGen, int gen);
void traceLiensIndividu(individual *id, Indiv3D indiv_chosen, int* tabGen, Indiv3D **indiv_coords, int gen_chose );
void traceLiensMariages(individual *id, Indiv3D indiv_chosen, int* tabGen, Indiv3D **indiv_coords, int gen_chose );
