///////////////////////////////////////////////////////////////////////////////
// IUT d'Aix-Marseille Universit�, site d'Arles
// D�partement Informatique
// 2�me ann�e
// ----------------------------------------------------------------------------
// IN - Synth�se d'images - Rendu r�aliste
// Auteur : S�bastien Thon
// ----------------------------------------------------------------------------
// Base du TP 1
// Base d'un programme permettant d'afficher une sc�ne compos�e d'un sol, de
// maisons et d'arbres. On placera des sources de lumi�re, on sp�cifiera les
// mat�riaux des diff�rents objets et on utilisera un effet de brouillard.
// ----------------------------------------------------------------------------
// Compilation sous Windows :
//   g++ -Wall jeu.cpp -o jeu.exe -lglut32 -lglu32 -lopengl32 -L.
// Compilation sous Linux :
//   g++ -Wall jeu.cpp -o jeu.exe -lglut -lGLU -lGL -L.
// Compilation sous MacOS :
//   g++ -framework GLUT -framework OpenGL jeu.cpp -o jeu.exe
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>
#include <vector>
#include "glut.h"
#include "struct.h"


using namespace std;

#define WIDTH   800										// Largeur de la fen�tre OpenGL
#define HEIGHT  600										// Hauteur de la fen�tre OpenGl

#define KEY_ESC 27										// Code ASCII de la touche Echap

float	xpos = 0, zpos = -50;							// Position de la cam�ra
int		angle_x = 0, angle_y = 0;						// Orientation de la cam�ra
int		mouse_x = 0, mouse_y = 0;						// Position de la souris
float lastx, lasty;

struct balle{
	float x,y,z;
};

vector<balle> balles;


GLvoid initGL()
{
	glClearColor(0, 0, 0, 1);							// Couleur servant � effacer la fen�tre (noir)
    glShadeModel(GL_SMOOTH);							// Mod�le d'ombrage : lissage de Gouraud
	glEnable(GL_CULL_FACE);								// Ne traite pas les faces cach�es
	glEnable(GL_DEPTH_TEST);							// Active le Z-Buffer
	glDepthFunc(GL_LEQUAL);								// Mode de fonctionnement du Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Active la correction de perspective (pour ombrage, texture, ...)
}


void affiche_paquet( float xp, float yp, float zp, float yr )
{
	glPushMatrix();										// Sauve la matrice de vue actuelle
	glTranslatef(xp, yp, zp);							// Positionne la maison avec une translation
	glRotatef(yr, 0,1,0);								// et une rotation

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
	affiche_paquet( 0, 0, 0, 0);

	for(unsigned int i=0; i < balles.size(); i++)
	{
		affiche_balle(balles[i].x, balles[i].y, balles[i].z, 0);
		cout << "Balle : X=" << balles[i].x << " | Y=" << balles[i].y << " | Z=" << balles[i].z << endl;
	}

	glutSwapBuffers();							// Affiche la sc�ne � l'�cran (affichage en double buffer)
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : affichage dans la fen�tre.
//-----------------------------------------------------------------------------
// Param�tres :
//    _
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_display()
{
	// On efface les pixels de l'image (color buffer) et le Z-Buffer (depth buffer).
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// On initialise la matrice de vue avec la matrice identit�.
	glLoadIdentity();

	// On applique une translation et une rotation � la sc�ne pour simuler
	// un d�placement de cam�ra.
	glTranslatef(xpos,0,zpos);
	glRotatef(angle_x, 1,0,0);
	glRotatef(angle_y, 0,1,0);

	//*****************************************************************
	//* A FAIRE :
	//* Re-sp�cifier la position des sources de lumi�re avec glLightfv()
	//* pour qu'elles soient aussi affect�es par la translation et la
	//* rotation qu'on vient d'appliquer � la sc�ne.
	//*****************************************************************



	// On affiche la sc�ne.
	affiche_scene();

	// On force OpenGL � afficher avant de passer � la suite.
	glFlush();
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : fonction appel�e lorsqu'on redimensionne la fen�tre.
//-----------------------------------------------------------------------------
// Param�tres :
//    width, height (in) : nouvelles largeur et hauteur de la fen�tre.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_reshape(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des �v�nements clavier.
//-----------------------------------------------------------------------------
// Param�tres :
//    key (in) : code ascii de la touche utilis�e.
//    x,y (in) : coordonn�es de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case KEY_ESC:						// 'ECHAP' :
			exit(1);						// on quitte le programme
			break;

		case 't':
			balle temp;
			temp.x = xpos;
			temp.z = zpos;
			balles.push_back(temp);
			cout << "-------" << endl;
			break;
	}
	glutPostRedisplay();
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des touches speciales du clavier.
//-----------------------------------------------------------------------------
// Param�tres :
//    key (in) : code ascii de la touche utilis�e.
//    x,y (in) : coordonn�es de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_special(int key, int x, int y)
{
	float speed = 1.0f;						// Vitesse de d�placement de la cam�ra

	switch (key)
	{
		case GLUT_KEY_UP:					// Fl�che vers le haut : 
			zpos += speed;					// on d�place la cam�ra selon z-
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_DOWN:					// Fl�che vers le bas :
			zpos -= speed;					// on d�place la cam�ra selon z+
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_LEFT:					// Fl�che vers la gauche :
			xpos += speed;					// on d�place la cam�ra selon x-
			glutPostRedisplay();			// et on demande le r�affichage.
			break;

		case GLUT_KEY_RIGHT:				// Fl�che vers la droite :
			xpos -= speed;					// on d�place la cam�ra selon x+
			glutPostRedisplay();			// et on demande le r�affichage.
			break;
	}
}

void mouvement_souris(int x, int y) 
{
    int diffx=x-lastx; //check the difference between the current x and the last x position
    int diffy=y-lasty; //check the difference between the current y and the last y position
    lastx=x; //set lastx to the current x position
    lasty=y; //set lasty to the current y position
    angle_x += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
    angle_y += (float) diffx;    //set the xrot to yrot with the addition of the difference in the x position

    glutPostRedisplay();
}



///////////////////////////////////////////////////////////////////////////////
// Call-back : gestion des d�placements de la souris.
//-----------------------------------------------------------------------------
// Param�tres :
//    x,y    (in) : coordonn�es de la souris.
// Retour :
//    _
///////////////////////////////////////////////////////////////////////////////
GLvoid callback_motion(int x, int y)
{
	angle_x += y - mouse_y;						// Modifie la direction de vue de la cam�ra
	angle_y += x - mouse_x;						// en fonction de la position de la souris
	mouse_x = x;
	mouse_y = y;
	glutPostRedisplay();						// Demande le r�affichage
}


///////////////////////////////////////////////////////////////////////////////
// Fonction principale du programme.
//-----------------------------------------------------------------------------
// Param�tres :
//    argc (in) : nombre d'arguments de la ligne de commande.
//    argv (in) : tableau de cha�nes de caract�res contenant les arguments.
// Retour :
//    Un entier contenant le code de retour du programme.
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	// Initialisation de param�tres de Glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Jeu");

	// Intitialisation de param�tres d'OpenGL
	initGL();

	// D�claration des fonctions de call-back.
	glutDisplayFunc(&callback_display);
	glutReshapeFunc(&callback_reshape);
	glutKeyboardFunc(&callback_keyboard);
	glutSpecialFunc(&callback_special);
	glutMotionFunc(&callback_motion);

	glutPassiveMotionFunc(&mouvement_souris);

	// Boucle principale de Glut.
	glutMainLoop();

	// Code de retour
	return 1;
}
