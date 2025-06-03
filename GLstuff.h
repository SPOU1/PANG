
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// para linux MAC
// #include "GLUT/glut.h"

// Para linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Prototipos de funciones
GLuint LoadTexture (int, int, const char *filename);

void displaytext (GLuint Thetexture);

void init_GL_stuff (char *, int, int);
