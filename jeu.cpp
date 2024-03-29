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
#include <vector>

#include "struct.h"
#include "Camera.hpp"

using namespace std;

#define ESC 27

//Position de la cible
float x = 0;
float y = 0;
float z = -10.0f;

//Sens du mouvement de la cible
bool sens = 0;

//Vitesse de la cible
float vitesse = 0.005f;

//Vitesse de la balle
float vitesseBalle = 0.1f;

//Sensibilit� de la souris
float vSouris = 0.2f;

struct balle{
	float x,y,z;
};

//Dimensions de la fen�tre
float width = 400;
float height = 400;

vector<balle> balles;
//vector<point3> vecteurB;
//int nbBalle = 0;

//Coordon�es de la cam�ra
double z_eye = 5;

double x_at = 0;
double y_at = 0;
double z_at = 0;

point3 eye(10, 5,z_eye);
point3 at(0,0,0);
point3 up(0,1,0);

//Cr�ation de la cam�ra
Camera camera(eye, at, up);

GLvoid initGL()
{
	glClearColor(0, 0, 0, 1);							// Couleur servant � effacer la fen�tre (noir)
    glShadeModel(GL_SMOOTH);							// Mod�le d'ombrage : lissage de Gouraud
	glEnable(GL_CULL_FACE);								// Ne traite pas les faces cach�es
	glEnable(GL_DEPTH_TEST);							// Active le Z-Buffer
	glDepthFunc(GL_LEQUAL);								// Mode de fonctionnement du Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Active la correction de perspective (pour ombrage, texture, ...)
}

//Affiche la cible
void affiche_paquet( float xp, float yp, float zp, float yr )
{
	glPushMatrix();
	glTranslatef(xp-2.75f, yp, zp);	
	glRotatef(yr, 0,0,0);

	//Dimensions du paquet
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

	glPopMatrix();
}

//Affiche une balle
void affiche_balle( float xp, float yp, float zp, float yr )
{
	glPushMatrix();
	glTranslatef(xp, yp, zp);
	glRotatef(yr, 0,1,0);

	glColor3f(1.0f,1.0f,0.0f);

	glutSolidSphere(0.5, 50, 50);

	glPopMatrix();
}

void affiche_scene()
{
  //z_eye += 0.005;

  //Initialisation de la cam�ra
  point3 neweye(0, 10, 10);

  point3 newat(x_at, y_at, z_at);
  camera.update(neweye, newat, up);
  camera.set();

  //Affichage de la cible
	affiche_paquet( x, y, z, 0);

	//Affichage des balles
	for(unsigned int i=0; i < balles.size(); i++)
	{
		affiche_balle(balles[i].x, balles[i].y, balles[i].z, 0);
		//cout << "Balle : X=" << balles[i].x << " | Y=" << balles[i].y << " | Z=" << balles[i].z << endl;
	}

	glutSwapBuffers();

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
   gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();


}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {

	//Quitte le programme
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
		//Destruction de la balle quand elle est trop loin
		if(balles[i].z <= -100)
		{
			balles.erase(balles.begin()+i);
			//cout << "Balle n" << i << " d�truite !" << endl;
		}
		//balles[i].x+=vitesseBalle;
		//balles[i].y+=vitesseBalle;
		balles[i].z-=vitesseBalle;
		//cout << "Balle n�" << i << " | X=" << balles[i].x << " Y=" << balles[i].y << " Z=" << balles[i].z << endl;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	//Tire une balle avec le clique gauche
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//Cr�ation de la balle sur la position de la cam�ra
		balle b;
		b.x = camera.at.x-camera.eye.x;
		b.y = camera.at.y-camera.eye.y;
		b.z = camera.at.z-camera.eye.z;
		balles.push_back(b);

		//point3 pt = point3(camera.at.x-camera.eye.x, camera.at.y-camera.eye.y, camera.at.z-camera.eye.z);
		//vecteurB.push_back(pt);
		//cout << "Balle n�" << nbBalle++ << " | X=" << b.x << " Y=" << b.y << " Z=" << b.z << endl;
	}
}

GLvoid callback_motion(int x, int y)
{
	//Mouvement de la cam�ra avec la souris
	int w = glutGet( GLUT_WINDOW_WIDTH );
	int h = glutGet( GLUT_WINDOW_HEIGHT );
	if(x > w/2)
	{
		x_at+=vSouris;
		//cout << "x++" << endl;
		glutWarpPointer( w / 2, h / 2 );
	}
	else if(x < w/2)
	{
		x_at-=vSouris;
		//cout << "x--" << endl;
		glutWarpPointer( w / 2, h / 2 );
	}
	else if(y < h/2)
	{
		y_at+=vSouris;
		//cout << "y++" << endl;
		glutWarpPointer( w / 2, h / 2 );
	}
	else if(y > h/2)
	{
		y_at-=vSouris;
		//cout << "y--" << endl;
		glutWarpPointer( w / 2, h / 2 );
	}

  glutPostRedisplay();
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
  //glutMotionFunc(&callback_motion);
  glutPassiveMotionFunc(&callback_motion);
	glutIdleFunc(&idle);
	glutMainLoop();

	return(EXIT_SUCCESS);
}
