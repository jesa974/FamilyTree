#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/aide.h"
#include "../../include/mariage.h"
#include "../../include/createIndividual.h"
#include "../../include/chaine.h"
#include "../../include/ia.h"


int compteMariage(weddLink *ptrTete) {
	
	weddLink *listeWedd = ptrTete;
	int total = 0;
	
	while(listeWedd != NULL) {
		
		total++;
		listeWedd = listeWedd->next;
		
	}
	
	return total;
}


int compteEnfants(childLink *ptrTete, individual *indiv) {
	
	childLink *liste = ptrTete;
	int total = 0;
	
	while(liste != NULL) {
		
		if(compChaine(liste->child->infos.name,indiv->infos.name) != 0) {
			total++;
		}
		
		liste = liste->next;
	}
	
	return total;
}


char **findTabVilleEnfants(childLink *ptrTete, char **tabVille) {
	
	childLink *liste = ptrTete;
	int cpt = 0;
	
	while(liste != NULL) {
		
		copieChaine(tabVille[cpt],liste->child->infos.birth.location);
		
		cpt++;
		
		liste = liste->next;
	}
	
	return tabVille;
}


char *findBirthLocationWedding(individual *person, weddLink *ptrTete) {
	
	weddLink *liste = ptrTete;
	char *result = "";
	
	while(liste != NULL) {
		
		if(person->father != NULL && person->mother != NULL) {
			
			if(compChaine(person->father->infos.name,liste->mariage->info_wedding.husband->infos.name) == 0 && compChaine(person->father->infos.surname,liste->mariage->info_wedding.husband->infos.surname) == 0 && compChaine(person->mother->infos.name,liste->mariage->info_wedding.wife->infos.name) == 0 && compChaine(person->mother->infos.surname,liste->mariage->info_wedding.wife->infos.surname) == 0) {
				
				if(compChaine(liste->mariage->info_wedding.wedd.location,"") != 0) {
					
					copieChaine(result,liste->mariage->info_wedding.wedd.location);
					break;
					
				}
				
			} else {
				
				liste = liste->next;
				
			}
			
		}
		
	}
	
	return result;
}


void findBirthDateWedding(individual *person, weddLink *ptrTete) {
	
	weddLink *liste = ptrTete;
	
	while(liste != NULL) {
		
		if(person->father != NULL && person->mother != NULL) {
			
			if(compChaine(person->father->infos.name,liste->mariage->info_wedding.husband->infos.name) == 0 && compChaine(person->father->infos.surname,liste->mariage->info_wedding.husband->infos.surname) == 0 && compChaine(person->mother->infos.name,liste->mariage->info_wedding.wife->infos.name) == 0 && compChaine(person->mother->infos.surname,liste->mariage->info_wedding.wife->infos.surname) == 0) {
				
				if(dateNulle(liste->mariage->info_wedding.wedd.time) != 0) {
					
					person->infos.birth.time.day = liste->mariage->info_wedding.wedd.time.day;
					
					person->infos.birth.time.month = liste->mariage->info_wedding.wedd.time.month;
					
					person->infos.birth.time.year = liste->mariage->info_wedding.wedd.time.year;
					
					break;
					
				}
				
			} else {
				
				liste = liste->next;
				
			}
			
		}
		
	}
	
}


void findBirthDateDeath(individual *person, date dead) {
	
	char *indivAge = NULL;
	int age = 0;
	
	indivAge = (char*)malloc(sizeof(char));
	indivAge[0] = person->infos.gender;
	
	if(dead.year < 1700) {
		
		if(compChaine(indivAge,"M") == 0) {
			
		} else {
			
		}
		
	} else if(dead.year < 1740) {
		
		if(compChaine(indivAge,"M") == 0) {
			
		} else {
			
		}
		
	} else if(dead.year < 1770) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 25;
		} else {
			age = 28;
		}
		
	} else if(dead.year < 1800) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 29;
		} else {
			age = 30;
		}
		
	} else if(dead.year < 1830) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 36;
		} else {
			age = 37;
		}
		
	} else if(dead.year < 1860) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 38;
		} else {
			age = 40;
		}
		
	} else if(dead.year < 1900) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 41;
		} else {
			age = 44;
		}
		
	} else if(dead.year < 1930) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 50;
		} else {
			age = 55;
		}
		
	} else if(dead.year < 1960) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 62;
		} else {
			age = 69;
		}
		
	} else if(dead.year < 1990) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 69;
		} else {
			age = 77;
		}
		
	} else if(dead.year < 2018) {
		
		if(compChaine(indivAge,"M") == 0) {
			age = 73;
		} else {
			age = 82;
		}
		
	}
	
	person->infos.birth.time.year = person->infos.death.time.year - age;
	person->infos.birth.time.month = person->infos.death.time.month;
	person->infos.birth.time.day = person->infos.death.time.day;
	
}


char *findBirthLocationChild(int nbFreres, childLink *listeEnfants) {
	
	char **tabChaine = (char**)malloc(sizeof(char*)*nbFreres);
	char *ville = "";
	int cptVille = 0;
	
	tabChaine = findTabVilleEnfants(listeEnfants,tabChaine);

	for(int i = 0;i < nbFreres; i++) {
		
		cptVille = 0;

		for(int k = 0;k < nbFreres; k++) {
	
			if(i != k && compChaine(tabChaine[i],tabChaine[k]) == 0) {
				cptVille++;	
			}
	
			if(cptVille >= (nbFreres/2)) {
				copieChaine(ville,tabChaine[i]);
			}
		}

		if(cptVille >= (nbFreres/2)) {
			break;
		}
	}

	if(tabChaine != NULL) {
		free(tabChaine);
		tabChaine = NULL;
	}
	
	return ville;
}


void find_lieu_naissance(individual *ptrTete) {
	
	individual *listeChaine = ptrTete;
	weddLink *listeWeddP = NULL;
	weddLink *listeWeddM = NULL;
	childLink *listeEnfantsP = NULL;
	childLink *listeEnfantsM = NULL;
	childLink *listeChild = NULL;
	char *lieuN;
	int nbFreresP = 0;
	int nbFreresM = 0;
	int nbWeddP = 0;
	int nbWeddM = 0;
	int nbChild = 0;
	
	lieuN = (char*)malloc(sizeof(char));
	copieChaine(lieuN,"");
	
	while(listeChaine != NULL) {
		
		if(compChaine(listeChaine->infos.birth.location,"") == 0) {
			
			printf("nom : %s\n",listeChaine->infos.name);
			
			listeChild = listeChaine->firstChild;
			
			if(listeChaine->father != NULL || listeChaine->mother != NULL) {
				
				if(listeChaine->father != NULL) {
					
					listeWeddP = listeChaine->father->firstUnion;
					
					nbWeddP = compteMariage(listeWeddP);
					
					listeEnfantsP = listeChaine->father->firstChild;
					
					nbFreresP = compteEnfants(listeEnfantsP,listeChaine);
					
				}
				
				if(listeChaine->mother != NULL) {
					
					listeWeddM = listeChaine->mother->firstUnion;
					
					nbWeddM = compteMariage(listeWeddM);
					
					listeEnfantsM = listeChaine->mother->firstChild;
					
					nbFreresM = compteEnfants(listeEnfantsM,listeChaine);
					
				}
				
				// En fonction du mariage
				if(listeWeddP != NULL || listeWeddM != NULL) {
					
					if((nbWeddP + nbWeddM) > 0 && nbWeddP != nbWeddM) {
						
						if(nbWeddP > nbWeddM && nbWeddP > 0) {
							
							if(nbWeddP > 1) {
								
								copieChaine(lieuN,findBirthLocationWedding(listeChaine,listeWeddP));
								
							} else {
								
								copieChaine(lieuN,listeWeddP->mariage->info_wedding.wedd.location);
								
							}
							
						} else if(nbWeddM > nbWeddP && nbWeddM > 0) {
							
							if(nbWeddM > 1) {
								
								copieChaine(lieuN,findBirthLocationWedding(listeChaine,listeWeddM));
								
							} else {
								
								copieChaine(lieuN,listeWeddM->mariage->info_wedding.wedd.location);
								
							}
							
						}
						
					} else if(nbWeddP == nbWeddM) {
						
						copieChaine(lieuN,listeWeddP->mariage->info_wedding.wedd.location);
						
					}
					
				}
				
				if(compChaine(lieuN,"") != 0) {
					copieChaine(listeChaine->infos.birth.location,lieuN);
					printf("lieu naissance mariage : %s\n",lieuN);
					break;
				}
				
				// En fonction des frères et soeurs
				if((nbFreresP + nbFreresM) > 0) {
					
					if(nbFreresP > nbFreresM && nbFreresP > 0) {
					
						copieChaine(lieuN,findBirthLocationChild(nbFreresP,listeEnfantsP));
					
					} else if(nbFreresM > nbFreresP && nbFreresM > 0) {
						
						copieChaine(lieuN,findBirthLocationChild(nbFreresM,listeEnfantsM));
					
					}
				
				}
				
				if(compChaine(lieuN,"") != 0) {
					copieChaine(listeChaine->infos.birth.location,lieuN);
					printf("lieu naissance freres : %s\n",lieuN);
					break;
				}
				
				// En fonction des parents
				if(listeChaine->father != NULL && listeChaine->mother != NULL && compChaine(listeChaine->father->infos.birth.location,listeChaine->mother->infos.birth.location) == 0) {
					
					copieChaine(lieuN,listeChaine->father->infos.birth.location);
					
				}
				
				if(compChaine(lieuN,"") != 0) {
					copieChaine(listeChaine->infos.birth.location,lieuN);
					printf("lieu naissance parents : %s\n",lieuN);
					break;
				}
				
			}
			
			// En fonction des enfants
			if(listeChild != NULL) {
				
				while(listeChild != NULL) {
					
					nbChild = compteEnfants(listeChild,listeChaine);
					
					if(nbChild > 1) {
						
						if(compChaine(findBirthLocationChild(nbChild,listeChild),"") != 0) {
							
							copieChaine(lieuN,findBirthLocationChild(nbChild,listeChild));
							break;
							
						} else {
							
							listeChild = listeChild->next;
							
						}
						
					} else {
						
						if(compChaine(listeChild->child->infos.birth.location,"") != 0 && listeChild->child->infos.birth.time.year < 1950) {
							
							copieChaine(lieuN,listeChild->child->infos.birth.location);
							break;
							
						} else {
							
							if(listeChild->child->infos.birth.time.year < 1950) {
								
								listeChild = listeChild->child->firstChild;
								
							} else {
								
								break;
								
							}
						
						}
						
					}
				
				}
				
			}
			
			if(compChaine(lieuN,"") != 0) {
				
				copieChaine(listeChaine->infos.birth.location,lieuN);
				printf("lieu naissance enfants : %s\n",lieuN);
			}
				
		}
		
		if(lieuN != NULL) {
			free(lieuN);
			lieuN = (char*)malloc(sizeof(char));
			copieChaine(lieuN,"");
		}
		
		listeChaine = listeChaine->next;
		
	}
}


void find_date_naissance(individual *ptrTete) {
	
	individual *listeChaine = ptrTete;
	weddLink *listeWeddP = NULL;
	weddLink *listeWeddM = NULL;
	//childLink *listeEnfantsP = NULL;
	//childLink *listeEnfantsM = NULL;
	//childLink *listeChild = NULL;
	//int nbFreresP = 0;
	//int nbFreresM = 0;
	int nbWeddP = 0;
	int nbWeddM = 0;
	//int nbChild = 0;
	
	while(listeChaine != NULL) {
		
		if(dateNulle(listeChaine->infos.birth.time) == 0) {
			
			printf("nom : %s\n",listeChaine->infos.name);
			
			//listeChild = listeChaine->firstChild;
			
			if(listeChaine->father != NULL || listeChaine->mother != NULL) {
				
				if(listeChaine->father != NULL) {
					
					listeWeddP = listeChaine->father->firstUnion;
					
					nbWeddP = compteMariage(listeWeddP);
					
					//listeEnfantsP = listeChaine->father->firstChild;
					
					//nbFreresP = compteEnfants(listeEnfantsP,listeChaine);
					
				}
				
				if(listeChaine->mother != NULL) {
					
					listeWeddM = listeChaine->mother->firstUnion;
					
					nbWeddM = compteMariage(listeWeddM);
					
					//listeEnfantsM = listeChaine->mother->firstChild;
					
					//nbFreresM = compteEnfants(listeEnfantsM,listeChaine);
					
				}
				
				// En fonction du mariage
				if(listeWeddP != NULL || listeWeddM != NULL) {
					
					if((nbWeddP + nbWeddM) > 0 && nbWeddP != nbWeddM) {
						
						if(nbWeddP > nbWeddM && nbWeddP > 0) {
							
							if(nbWeddP > 1) {
								
								findBirthDateWedding(listeChaine,listeWeddP);
								
							}
							
						} else if(nbWeddM > nbWeddP && nbWeddM > 0) {
							
							if(nbWeddM > 1) {
								
								findBirthDateWedding(listeChaine,listeWeddM);
								
							}
							
						}
						
					} else if(nbWeddP == nbWeddM) {
						
						findBirthDateWedding(listeChaine,listeWeddP);
						
					}
					
				}
				
				if(dateNulle(listeChaine->infos.birth.time) != 0) {
					printf("date naissance mariage : %d/%d/%d\n",listeChaine->infos.birth.time.day,listeChaine->infos.birth.time.month,listeChaine->infos.birth.time.year);
					break;
				}
				
			}
			
			// En fonction de la date de mort
			if(dateNulle(listeChaine->infos.death.time) != 0) {
				
				findBirthDateDeath(listeChaine,listeChaine->infos.death.time);
				
			}
			
			if(dateNulle(listeChaine->infos.birth.time) != 0) {
				printf("date naissance mort : %d/%d/%d\n",listeChaine->infos.birth.time.day,listeChaine->infos.birth.time.month,listeChaine->infos.birth.time.year);
				break;
			}
				
		}
		
		listeChaine = listeChaine->next;
		
	}
	
}


int testMort_avecDate(individual *ptr){
	int etat = 0;
	
	if(ptr->infos.death.time.year != 0){
		if(ptr->infos.birth.time.year<=1600){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=19){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1600 && ptr->infos.birth.time.year<=1740){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=26){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1740 && ptr->infos.birth.time.year<=1770){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=25){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1770 && ptr->infos.birth.time.year<=1800){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=29){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1800 && ptr->infos.birth.time.year<=1830){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=36){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1830 && ptr->infos.birth.time.year<=1860){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=38){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1860 && ptr->infos.birth.time.year<=1900){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=41){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1900 && ptr->infos.birth.time.year<=1930){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=50){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1930 && ptr->infos.birth.time.year<=1960){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=62){
				etat = 1;
			}
		}
		
		if(ptr->infos.birth.time.year>1960 && ptr->infos.birth.time.year<=1990){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=69){
				etat = 1;
			}
		}
		if(ptr->infos.birth.time.year>1990){
			if(ptr->infos.death.time.year-ptr->infos.birth.time.year>=73){
				etat = 1;
			}
		}
	}else if(ptr->infos.birth.time.year<=1939){
		etat= 1;
	}
	
	return etat;
}


void find_lieu_mort(individual *ptrTete){
	individual *ptr=ptrTete;
	weddLink *maria;
	individual *conjoint;
	
	while(ptr!=NULL){
		
		
		if(compChaine(ptr->infos.death.location,"")==0 && testMort_avecDate(ptr)==1){ //&& 
			printf("je rentre pour : %s, %s\n",ptr->infos.name,ptr->infos.surname);
			
			if(ptr->firstUnion!=NULL){
				
				maria=ptr->firstUnion;
				
				while(maria!=NULL){
					
					
					if(maria->mariage->info_wedding.husband==ptr){
						conjoint=maria->mariage->info_wedding.wife;
					
					}else if(maria->mariage->info_wedding.wife==ptr){
						conjoint=maria->mariage->info_wedding.husband;
					}
					
					if(compChaine(conjoint->infos.death.location,"")==-1){
						ptr->infos.death.location=conjoint->infos.death.location;
					}
					
					maria=maria->next;
				}
			}
			
			if(compChaine(ptr->infos.death.location,"")==0){
				ptr->infos.death.location=ptr->infos.birth.location;
			}
		}
		
		
		ptr=ptr->next;
		
	}
}



void find_lieu_mariage(wedding *ptrTete){
	wedding *ptr=ptrTete;
	weddLink *ptr2;

	while(ptr!=NULL){
		if(compChaine(ptr->info_wedding.wedd.location,"")==0){
			
			if(ptr->info_wedding.husband->firstUnion !=NULL){
				ptr2=ptr->info_wedding.husband->firstUnion;
				
				while(ptr2!=NULL){
					if(compChaine(ptr2->mariage->info_wedding.wedd.location,"")==-1){
						ptr->info_wedding.wedd.location=ptr2->mariage->info_wedding.wedd.location;
					}
					ptr2=ptr2->next;
				}
			}
		}
		
		if(compChaine(ptr->info_wedding.wedd.location,"")==0){
			ptr->info_wedding.wedd.location=ptr->info_wedding.husband->infos.birth.location;
		}
		
		ptr=ptr->next;
	}
}

void find_date_mort(individual *ptrTete){
	individual *ptr=ptrTete;
	//char gender = (char)"M";
	while(ptr!=NULL){
		
		if(ptr->infos.death.time.year!=0 && testMort_avecDate(ptr)==1 ){ //&& ptr->infos.gender==gender
			printf("je rentre en toi pour : %s, %s\n",ptr->infos.name,ptr->infos.surname);
			if(ptr->infos.birth.time.year<=1600){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+19;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
					
			}
			
			if(ptr->infos.birth.time.year>1600 && ptr->infos.birth.time.year<=1740){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+26;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1740 && ptr->infos.birth.time.year<=1770){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+25;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1770 && ptr->infos.birth.time.year<=1800){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+29;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1800 && ptr->infos.birth.time.year<=1830){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+36;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1830 && ptr->infos.birth.time.year<=1860){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+38;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1860 && ptr->infos.birth.time.year<=1900){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+41;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1900 && ptr->infos.birth.time.year<=1930){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+50;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1930 && ptr->infos.birth.time.year<=1960){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+62;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			
			if(ptr->infos.birth.time.year>1960 && ptr->infos.birth.time.year<=1990){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+69;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			if(ptr->infos.birth.time.year>1990){
				ptr->infos.death.time.year=ptr->infos.birth.time.year+73;
				ptr->infos.death.time.month=ptr->infos.birth.time.month;
				ptr->infos.death.time.day=ptr->infos.birth.time.day;
			}
			printf("je rentre en toi mort pour : %s, %s, %d\n",ptr->infos.name,ptr->infos.surname,ptr->infos.death.time.year);
		}
		
		ptr=ptr->next;
	}
}

void find_date_mariage(wedding *ptrTete){
	wedding *ptr = ptrTete;
	individual *husband;
	individual *wife; //peut etre devoir faire les malloc
	childLink *Cptr1;
	childLink *Cptr2;
	individual *child;
	
	
	while(ptr!=NULL){
		if(ptr->info_wedding.wedd.time.day == 0){
			husband = malloc(sizeof(individual));
			wife = malloc(sizeof(individual));
			
			husband = ptr->info_wedding.husband;
			wife = ptr->info_wedding.wife;
			
			if(husband->firstChild!=NULL && wife->firstChild!=NULL){
				//Cptr1=malloc(sizeof(childLink));
				//Cptr2=malloc(sizeof(childLink));
				Cptr2=wife->firstChild;
				Cptr1=husband->firstChild;
				
				while(Cptr1!=NULL){
					while(Cptr2!=NULL){
						if(Cptr1->child==Cptr2->child){
							child = Cptr1->child;
							break;
						}
						Cptr2=Cptr2->next;
					}
					Cptr1=Cptr1->next;
				}
				ptr->info_wedding.wedd.time=child->infos.birth.time;
				ptr->info_wedding.wedd.time.year=ptr->info_wedding.wedd.time.year-1;
			}
		}
		
		if(ptr->info_wedding.wedd.time.day == 0){
			ptr->info_wedding.wedd.time=ptr->info_wedding.husband->infos.birth.time;
			ptr->info_wedding.wedd.time.year=ptr->info_wedding.husband->infos.birth.time.year+30;
		}
		
		
		ptr=ptr->next;
	}
}
