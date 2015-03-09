#Makefile Linux
EXEC = bezier
COMPILO = g++

GLUT_INC=
GLUT_LIBS=-lglut
GL_INC=
GL_LIBS= -lGL

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

$(EXEC): struct.o squelette_Bezier.o
	$(COMPILO) -o $@ $^ $(LIBS)

struct.o:  $(PATH_SRC)/struct.cpp $(PATH_INC)/struct.h
	$(COMPILO) -o $@ -c $< $(CXXFLAGS)

squelette_Bézier.o: squelette_Bezier.cpp $(PATH_INC)/struct.h
	$(COMPILO) -o $@ -c $< $(CXXFLAGS)

#Efface les fichiers .o
clean:
	rm -rf *.o
