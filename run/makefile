exec: main.o affichage.o arbre.o statistique.o diagramme.o chaine.o childLink.o weddLink.o ia.o mariage.o createIndividual.o textures.o ville.o carto.o interface.o buttons.o
	gcc -Wall main.o arbre.o affichage.o statistique.o diagramme.o chaine.o childLink.o weddLink.o ia.o mariage.o createIndividual.o textures.o ville.o carto.o interface.o buttons.o -o exec -lglut `sdl-config --cflags --libs` -lSDL_image -lm -lGLU -lGL

main.o: ../src/main.c ../include/My_GL.h ../include/aide.h ../include/arbre.h ../include/createIndividual.h ../include/statistique.h ../include/diagramme.h ../include/ia.h ../include/texture.h ../include/interface.h ../include/buttons.h
	gcc -c -Wall ../src/main.c

affichage.o: ../src/rendering/affichage.c ../include/My_GL.h ../include/aide.h
	gcc -c -Wall ../src/rendering/affichage.c

mariage.o: ../src/analysis/mariage.c ../include/mariage.h ../include/createIndividual.h ../include/aide.h ../include/chaine.h
	gcc -Wall -c ../src/analysis/mariage.c

createIndividual.o: ../src/analysis/createIndividual.c ../include/createIndividual.h ../include/aide.h ../include/chaine.h
	gcc -Wall -c ../src/analysis/createIndividual.c

chaine.o: ../src/analysis/chaine.c ../include/chaine.h
	gcc -Wall -c ../src/analysis/chaine.c

childLink.o: ../src/analysis/childLink.c ../include/childLink.h ../include/aide.h ../include/createIndividual.h
	gcc -Wall -c ../src/analysis/childLink.c

weddLink.o: ../src/analysis/weddLink.c ../include/weddLink.h ../include/aide.h
	gcc -Wall -c ../src/analysis/weddLink.c

arbre.o : ../src/rendering/arbre.c ../include/arbre.h ../include/aide.h ../include/My_GL.h
	gcc -c -Wall ../src/rendering/arbre.c
	
ia.o: ../src/analysis/ia.c ../include/ia.h ../include/aide.h ../include/mariage.h ../include/createIndividual.h ../include/chaine.h
	gcc -Wall -c ../src/analysis/ia.c
		
statistique.o : ../src/rendering/statistique.c ../include/statistique.h ../include/aide.h ../include/My_GL.h ../include/interface.h
	gcc -c -Wall ../src/rendering/statistique.c
	
diagramme.o : ../src/rendering/diagramme.c ../include/diagramme.h ../include/aide.h ../include/My_GL.h ../include/interface.h
	gcc -c -Wall ../src/rendering/diagramme.c
	
interface.o: ../src/rendering/interface.c  ../include/aide.h ../include/chaine.h  ../include/My_GL.h ../include/interface.h ../include/createIndividual.h
	gcc -c -Wall ../src/rendering/interface.c

textures.o: ../src/rendering/textures.c ../include/texture.h 
	gcc -c -Wall ../src/rendering/textures.c
	
ville.o: ../src/analysis/ville.c ../include/chaine.h ../include/ville.h
	gcc -c -Wall ../src/analysis/ville.c

carto.o: ../src/rendering/carto.c ../include/aide.h ../include/My_GL.h ../include/ville.h
	gcc -c -Wall ../src/rendering/carto.c
	
buttons.o : ../src/actions/buttons.c ../include/buttons.h ../include/ville.h ../include/childLink.h ../include/aide.h ../include/createIndividual.h ../include/statistique.h 
	gcc -c -Wall ../src/actions/buttons.c

clean:
	rm -f *~ *.o
