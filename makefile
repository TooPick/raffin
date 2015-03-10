#Makefile Linux
EXEC = jeu 
COMPILO = g++

GLUT_INC=
GLUT_LIBS=-lglut
GL_INC=
GL_LIBS= -lGL -lGLU -L.

#Chemin d'acces des includes
CXXINCS =  $(GLUT_INC) $(GL_INC)

#Parametres de compilation
CXXFLAGS = $(CXXINCS) -Wall

#Chemin d'acces des librairies
LIBS = $(GL_LIBS) $(GLUT_LIBS)

#Chemin d'acces des .h et .cpp (qui pourraient etre ailleurs que dans le répertoire principal)
PATH_INC=.
PATH_SRC=.

#Cree l'executable sous Linux
all: $(EXEC)

$(EXEC): jeu.o struct.o Camera.o
	$(COMPILO) -o $@ $^ $(LIBS)

jeu.o: $(PATH_SRC)/jeu.cpp 
	$(COMPILO) -o $@ -c $< $(CXXFLAGS)
Camera.o:  $(PATH_SRC)/Camera.cpp $(PATH_INC)/Camera.hpp
	$(COMPILO) -o $@ -c $< $(CXXFLAGS)

struct.o: struct.cpp $(PATH_INC)/struct.h
	$(COMPILO) -o $@ -c $< $(CXXFLAGS)

#Efface les fichiers .o
clean:
	rm -rf *.o
