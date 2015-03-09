/*
 *	Tableau des points permettant de g�rer les points de controles
 * On s�lectionne le point en se d�pla�ant avec + et -, ...
 * On s�lectionne ensuite si on veut faire monter, descendre amener vers la gauche ou la droite le point.
 *   d : translation � droite
 *   q : � gauche
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
//parce qu'il manque des choses aux gens qui utilisent MsWindows, o� OpenGL ne peut fonctionner que s'il discute avec le gestionnaire de fen�tres ("windows.h")
	#define GLUT_DISABLE_ATEXIT_HACK	//utile si vous compilez en g++ sous windows avec le glut.lib pr�vu pour MsVisual
	#include <windows.h>
	#include "glut.h"	//il faut avoir le fichier "glut.h" dans le m�me r�pertoire que votre .cpp (et aussi le glut.lib et le glut.dll)
#endif

#ifdef __linux__
	//pour les non-windowiens, GLUT (fen�trage opengl) est inclu dans les distributions
	//pour les ubuntiens : apt-get install freeglut3 freeglut3-dev
	#include <GL/glut.h>
#endif

#include <iostream>
#include <cstdlib> //pour exit(), et EXIT_SUCCESS

#include <cstdio>
#include <cmath>

#include "struct.h"
#include "Camera.hpp"

using namespace std;

#define ESC 27

float x = 0;
float y = 0;
float z = 0;
bool sens = 0;

double z_eye = 5;

point3 eye(10,0,z_eye);
point3 at(0,0,0);
point3 up(0,1,0);

Camera camera(eye, at, up);


void affiche_paquet( float xp, float yp, float zp, float yr )
{
	glPushMatrix();										// Sauve la matrice de vue actuelle
	glTranslatef(xp, yp, zp);							// Positionne la maison avec une translation
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
  z_eye++;
  point3 neweye(10,0, z_eye);
  camera.update(neweye, at, up);
  cout << z_eye << endl;
  camera.set();
	affiche_paquet( x, y, z, 0);
	/*
	for(unsigned int i=0; i < balles.size(); i++)
	{
		affiche_balle(balles[i].x, balles[i].y, balles[i].z, 0);
		cout << "Balle : X=" << balles[i].x << " | Y=" << balles[i].y << " | Z=" << balles[i].z << endl;
	}
	*/
	glutSwapBuffers();							// Affiche la sc�ne � l'�cran (affichage en double buffer)
}

/* initialisation d'OpenGL*/
static void init() {


}

/* Dessin */
void display(void) {

	// On efface les pixels de l'image (color buffer) et le Z-Buffer (depth buffer).
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// On initialise la matrice de vue avec la matrice identit�.
	glLoadIdentity();

	// On affiche la sc�ne.
	affiche_scene();

	// On force OpenGL � afficher avant de passer � la suite.
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

	default :
		break;

}

glutPostRedisplay();
}

void idle()
{
	x+=0.001f;
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
	glutIdleFunc(&idle);
	glutMainLoop();

	return(EXIT_SUCCESS);
}
