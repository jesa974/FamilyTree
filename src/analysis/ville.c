#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/ville.h"
#include "../../include/chaine.h"

#define URL_START "https://maps.googleapis.com/maps/api/geocode/xml?address="
#define URL_KEY "&key=AIzaSyAHI1t_v-quOwBr1T20VrbBqMxIshIqHmg"

int urlRequestFile(char* url);
GPS getLongLatFromXML(char *xml_fichier);

GPS trouveVilleGPS(char *ville_chose)
{
	GPS coords = {0,0};
	if(ville_chose != NULL && strcmp(ville_chose,"") ){
		printf(" -------------- CITY QUERY : %s -------------------\n",ville_chose);
		char *url = (char*) malloc(sizeof(char) * (strlen(URL_START) + strlen(ville_chose) + strlen(URL_KEY)));
		strcpy(url,URL_START);
		strcat(url,ville_chose);
		strcat(url,URL_KEY);
		
		urlRequestFile(url);
		
		coords = getLongLatFromXML("../ressources/villeInfos.xml");
	}
	
	return coords;
}

GPS getLongLatFromXML(char *xml_fichier){
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int cpt=0;
    int geomTest = 0;
    
    double lat=0;
    double longt=0;
    
    char buffLat[200];
    char buffLong[200];

    fp = fopen(xml_fichier, "rt");
    if (fp == NULL){
		printf("Fichier introuvable\n");
        exit(EXIT_FAILURE);
	}

    while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        //printf("%d :",cpt);
        if(strcmp(line, "  <geometry>\n")==0){
			geomTest=cpt;
			printf("found geometry balise at line %d\n",cpt);
		}
        if(geomTest>0 && cpt == geomTest+2){
			strcpy(buffLat,line);
			printf("Retrieved lat: %s", line);
		}
		if(geomTest>0 && cpt == geomTest+3){
			strcpy(buffLong,line);
			printf("Retrieved long: %s", line);
		}
        cpt++;
    }
    
    if(geomTest != -1){
		char *strlat = buffLat+9;
		strlat[strlen(strlat)-7] = '\0';
		printf("'%s'\n",strlat);
		char *strlong = buffLong+9;
		strlong[strlen(strlong)-7] = '\0';
		printf("'%s'\n",strlong);
		
		lat = atof(strlat);
		longt = atof(strlong);
		
		printf("latitude = %f longitude = %f\n",lat,longt);
	}
    
    fclose(fp);
    if (line)
        free(line);
    
    GPS coords;
    coords.lats = lat;
    coords.longs = longt;
    
    return coords;
}

void strappend(char* str, char c){
	int len = strlen(str);
	str[len] = c;
	str[len+1] = '\0';
}

int urlRequestFile(char* url){
	char command[512];
	strcpy(command,"curl --output ../ressources/villeInfos.xml '");
	//Analyse de la présence du caractère espace dans l'url : formatage en '%20'
	int i=0;
	while( url[i] != '\0')
	{
		if( url[i]== ' '){
			strcat(command,"%20");
		}else{
			strappend(command,url[i]);
		}
		i++;
	}
	strcat(command,"'");
	printf("trying command ' %s '\n",command);
	
	system(command);
	return EXIT_SUCCESS;
}
