#include "Camera.hpp"

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

Camera::Camera(point3 _eye, point3 _at, point3 _up) : eye(_eye), at(_at), up(_up)
{
  set();
}

void Camera::set()
{
  gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}

void Camera::update(point3 _eye, point3 _at, point3 _up)
{
  eye = _eye;
  at = _at;
  up = _up;
}
