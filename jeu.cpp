/*
 *	Tableau des points permettant de gérer les points de controles
 * On sélectionne le point en se déplaçant avec + et -, ...
 * On sélectionne ensuite si on veut faire monter, descendre amener vers la gauche ou la droite le point.
 *   d : translation à droite
 *   q : à gauche
 *   z : en haut
 *   s : en bas
 *
 */

// ----------------------------------------------------------------------------
// Compilation sous Windows :
//   g++ -Wall jeu.cpp -o jeu.exe -lglut32 -lglu32 -lopengl32 -L.
// Compilation sous Linux :
//   g++ -Wall jeu.cpp -o jeu.exe -lglut -lGLU -lGL -L.
// Compilation sous MacOS :
//   g++ -framework GLUT -framework OpenGL jeu.cpp -o jeu.exe
///////////////////////////////////////////////////////////////////////////////
 

#ifdef __APPLE__
//compilation sous MacOs par :
//gcc -framework GLUT -framework OpenGL -framework Cocoa source.c -o nom_executable
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/glut.h>
#endif

#ifdef __WIN32__
//parce qu'il manque des choses aux gens qui utilisent MsWindows, où OpenGL ne peut fonctionner que s'il discute avec le gestionnaire de fenêtres ("windows.h")
	#define GLUT_DISABLE_ATEXIT_HACK	//utile si vous compilez en g++ sous windows avec le glut.lib prévu pour MsVisual
	#include <windows.h>
	#include "glut.h"	//il faut avoir le fichier "glut.h" dans le même répertoire que votre .cpp (et aussi le glut.lib et le glut.dll)
#endif

#ifdef __linux__
	//pour les non-windowiens, GLUT (fenêtrage opengl) est inclu dans les distributions
	//pour les ubuntiens : apt-get install freeglut3 freeglut3-dev
	#include <GL/glut.h>
#endif

#include <iostream>
#include <cstdlib> //pour exit(), et EXIT_SUCCESS

#include <cstdio>
#include <cmath>
#include <vector>

using namespace std;

#define ESC 27

float x = 0;
float y = 0;
float z = 0;
bool sens = 0;
float vitesse = 0.005f;

struct balle{
	float x,y,z;
};

vector<balle> balles;
int nbBalle = 0;

void affiche_paquet( float xp, float yp, float zp, float yr )
{
	glPushMatrix();										// Sauve la matrice de vue actuelle
	glTranslatef(xp-2.75f, yp, zp);							// Positionne la maison avec une translation
	glRotatef(yr, 0,0,0);								// et une rotation

	GLdouble hauteurB = 2.5;
	GLdouble hauteur = 6.3;
	GLdouble largeur = 5.5;
	GLdouble profondeur = 2.2;

	glColor3f(1.0f,1.0f,1.0f);			// Couleur courante : blanc

	// Boite
	glBegin(GL_QUADS);

		glVertex3d(0, hauteur, 0);
		glVertex3d(largeur, hauteur, 0);
		glVertex3d(largeur, 0, 0);
		glVertex3d(0, 0, 0);

		glVertex3d(0, 0, 0);
		glVertex3d(0, 0, profondeur);
		glVertex3d(0, hauteur, profondeur);
		glVertex3d(0, hauteur, 0);

		glVertex3d(0, 0, profondeur);
		glVertex3d(largeur, 0, profondeur);
		glVertex3d(largeur, hauteur, profondeur);
		glVertex3d(0, hauteur, profondeur);
		
		glVertex3d(largeur, 0, 0);
		glVertex3d(largeur, hauteur, 0);
		glVertex3d(largeur, hauteur, profondeur);
		glVertex3d(largeur, 0, profondeur);

		glVertex3d(0, 0, 0);
		glVertex3d(largeur, 0, 0);
		glVertex3d(largeur, 0, profondeur);
		glVertex3d(0, 0, profondeur);
		
		glVertex3d(0, hauteur, profondeur);
		glVertex3d(largeur, hauteur, profondeur);
		glVertex3d(largeur, hauteur, 0);
		glVertex3d(0, hauteur, 0);

	glEnd();

	glColor3f(1.0f,0.0f,0.0f);			// Couleur courante : rouge

	// Bouchon
	glBegin(GL_QUADS);

		glVertex3d(0, hauteur+hauteurB, 0);
		glVertex3d(largeur, hauteur+hauteurB, 0);
		glVertex3d(largeur, hauteur, 0);
		glVertex3d(0, hauteur, 0);

		glVertex3d(0, hauteur, 0);
		glVertex3d(0, hauteur, profondeur);
		glVertex3d(0, hauteur+hauteurB, profondeur);
		glVertex3d(0, hauteur+hauteurB, 0);

		glVertex3d(0, hauteur, profondeur);
		glVertex3d(largeur, hauteur, profondeur);
		glVertex3d(largeur, hauteur+hauteurB, profondeur);
		glVertex3d(0, hauteur+hauteurB, profondeur);
		
		glVertex3d(largeur, hauteur, 0);
		glVertex3d(largeur, hauteur+hauteurB, 0);
		glVertex3d(largeur, hauteur+hauteurB, profondeur);
		glVertex3d(largeur, hauteur, profondeur);

		glVertex3d(0, hauteur, 0);
		glVertex3d(largeur, hauteur, 0);
		glVertex3d(largeur, hauteur, profondeur);
		glVertex3d(0, hauteur, profondeur);
		
		glVertex3d(0, hauteur+hauteurB, profondeur);
		glVertex3d(largeur, hauteur+hauteurB, profondeur);
		glVertex3d(largeur, hauteur+hauteurB, 0);
		glVertex3d(0, hauteur+hauteurB, 0);

	glEnd();

	glPopMatrix();										// Restaure la matrice de vue
}

void affiche_balle( float xp, float yp, float zp, float yr )
{
	glPushMatrix();										// Sauve la matrice de vue actuelle
	glTranslatef(xp, yp, zp);							// Positionne la maison avec une translation
	glRotatef(yr, 0,1,0);								// et une rotation

	glColor3f(1.0f,1.0f,0.0f);

	glutSolidSphere(0.5, 50, 50);

	glPopMatrix();		
}

void affiche_scene()
{
	
	affiche_paquet( x, y, z, 0);

	for(unsigned int i=0; i < balles.size(); i++)
	{
		affiche_balle(balles[i].x, balles[i].y, balles[i].z, 0);
		//cout << "Balle : X=" << balles[i].x << " | Y=" << balles[i].y << " | Z=" << balles[i].z << endl;
	}

	glutSwapBuffers();							// Affiche la scène à l'écran (affichage en double buffer)
}

/* initialisation d'OpenGL*/
static void init() {
	
}

/* Dessin */
void display(void) {

	// On efface les pixels de l'image (color buffer) et le Z-Buffer (depth buffer).
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// On initialise la matrice de vue avec la matrice identité.
	glLoadIdentity();

	// On affiche la scène.
	affiche_scene();

	// On force OpenGL à afficher avant de passer à la suite.
	glFlush();
	
}

/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-20.0f, 20.0f, -20.0f, 20.0f, -10.0f, 10.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	

	case ESC:
		exit(0);
		break;

	case 116:
		balle b;
		b.x = 0;
		b.y = 0;
		b.z = -15;
		balles.push_back(b);
		cout << "Balle n°" << nbBalle++ << " | X=" << b.x << " Y=" << b.y << " Z=" << b.z << endl;
		break;

	default :
		break;

}

glutPostRedisplay();
}

void idle()
{
	//Mouvement cible
	if(sens == 0 && x < 10)
	{
		x+=vitesse;
	}
	else if(sens == 0 && x >= 10)
	{
		sens = 1;
	}
	else if(sens == 1 && x > -10)
	{
		x-=vitesse;
	}
	else if(sens == 1 && x <= -10)
	{
		sens = 0;
	}

	//Mouvement balles
	for(unsigned int i = 0; i < balles.size(); i++)
	{
		if(balles[i].z >= 50)
		{
			balles.erase(balles.begin()+i);
			cout << "Balle n" << i << " détruite !" << endl;
		}
		balles[i].z+=vitesse;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		balle b;
		b.x = x;
		b.y = y;
		b.z = -15;
		balles.push_back(b);
		cout << "Balle n°" << nbBalle++ << " | X=" << b.x << " Y=" << b.y << " Z=" << b.z << endl;
	}
}

int main(int argc, char **argv) {
	glutInitWindowSize(400, 400);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Jeu de la mort qui tue");

	init();

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseFunc(&mouse);
	glutIdleFunc(&idle);
	glutMainLoop();

	return(EXIT_SUCCESS);
}
