//****************************************************//
// Segundo trabajo de Programación de Sistemas. PANG. //
// Iñaki García Valero, 22136                         //
// Steffen Pablo Otten, 22252                         //
// Javier Melero Moreno, 20216                        //
// Curso 2024-2025                                    //
//****************************************************//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include "Shape.h"
#include "Ball.h"
#include "Bullet.h"
#include "Man.h"
#include "Animal.h"
#include "ObjectsList.h"

#include "AudioPlayer.h"

#include "commonstuff.h"
#include "GLstuff.h"

//***********************
// Prototipos de funciones
//***********************

// Callback de la logica del juego
void myLogic();

// Callback de dibujo
void OnDibuja (void);

// Callback de high score
int getHighScore();

// Callbacks para teclado
void OnKeyboardDown (unsigned char key, int x, int y);
void OnSpecKeyboardDown (int key, int x, int y);
void OnKeyboardUp (unsigned char key, int x, int y);
void OnSpecKeyboardUp (int key, int x, int y);

// Callback de salida del juego
void exit_game();

// función para imprimir info en la pantalla
void printdata();

// ***********************
// Variables globales
// ***********************

bool keysHeld[256] = {false};
bool specialKeysHeld[256] = {false};

// Posición de la camara
float cam_pos[6] = {0, 0, 27};

// Objetos globales
ObjectsList worldobjects;  // lista de objetos del juego
Man *theMan = NULL;        // puntero al jugador
Bullet *theBullet = NULL;  // puntero al proyectil
Animal *theAnimal = NULL;  // puntero al animal (si hay uno)

// Varias constantes usadas en el programa
int shotTime = 0;   // contador de tiempo de vida del proyectil
int nMan = 5;       // numero de "vidas"
int score = 0;      // Puntuación
int highScore = 0;  // Puntuación más alta

int gameState = MENU_STATE;  // Estado del juego (MENU_STATE, GAME_STATE, GAMEOVER_STATE)

GLuint KLtexture;  // Texture datas tructure

//***********************
// Programa principal
//***********************

int main (int argc, char *argv[]) {
    // inicializaciones
    srandom (time (NULL));  // inicializa la semilla del generador de números aleatorios
    // Inicialización GLUT
    glutInit (&argc, argv);

    // Inicializaciones openGL (ver GLstuff.c)
    init_GL_stuff ((char *) "PANG 2025", WINX, WINY);

    // Define call backs te GLUT

    // Display function: contiene las instrucciones para dibujar los objetos
    glutDisplayFunc (OnDibuja);

    // Idle function: contiene la logica del juego
    glutIdleFunc (myLogic);

    // Callbacks de teclado
    glutKeyboardFunc (OnKeyboardDown);
    glutKeyboardUpFunc (OnKeyboardUp);
    glutSpecialFunc (OnSpecKeyboardDown);
    glutSpecialUpFunc (OnSpecKeyboardUp);

    atexit (exit_game);  // Callback para cuando se cierra la ventana

    // Posicciona el punto de vista (cámara)
    gluLookAt (
        cam_pos[0],
        cam_pos[1],
        cam_pos[2],  // posicion del ojo
        0.0,
        0.0,
        0.0,  // hacia que punto mira
        0.0,
        1.0,
        0.0);  // vector "UP"  (vertical positivo)

    // Carga la imagen del fondo
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    KLtexture = LoadTexture (800, 600, "PANG.bmp");
    glBindTexture (GL_TEXTURE_2D, KLtexture);

    // WORLDOBJECTS es una variable estática, se inicializa "automaticamente" al ejecutar el
    // programa o sea, su constructor es ejecutado al crear la variable cuando arranca el programa
    // Contiene los elementos del juego (punteros a los objetos)

    // Creacción de los objetos iniciales
    theMan = worldobjects.getMan();

    // Carga la puntuación más alta
    highScore = getHighScore();

    // Inicializa el reproductor de audio
    AudioPlayer::playMusic ("sounds/music.wav");

    // bucle infinito de Open GL
    glutMainLoop();

    // Esto solo sirve para que el compilador no proteste, nunca se llegará aquí
    return 0;
}

/************************** FIN DEL MAIN ************************************/

//***********************
// Callbacks
//***********************

// Imprime puntuacción y num. de naves
void printdata() {
    int i, l;
    char buffer[50];  // contiene la cadena de caracteres que se muestra

    glPushMatrix();

    sprintf (buffer, "Men: %d\t\t\tScore: %d\t\t\tHigh Score: %d", nMan, score, highScore);
    l = strlen (buffer);

    glDisable (GL_LIGHTING);
    glColor3f (1.0f, 1.0f, 1.0f);
    glRasterPos3f (-9, 9, -0.1);
    for (i = 0; i < l; i++) { glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, buffer[i]); }
    glEnable (GL_LIGHTING);

    glPopMatrix();
}

void drawBox (float x, float y, float width, float height, float r, float g, float b, float alpha) {
    glPushAttrib (GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);  // Guarda el estado actual de OpenGL
    glDisable (GL_LIGHTING);
    glDisable (GL_DEPTH_TEST);  // Desactiva la prueba de profundidad para elementos 2D

    // Cambia a proyección ortográfica
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D (0, WINX, 0, WINY);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Activa el blending para la transparencia
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Dibuja el rectángulo con el color y la transparencia especificados
    glColor4f (r, g, b, alpha);
    glBegin (GL_QUADS);
    glVertex2f (x, y);
    glVertex2f (x + width, y);
    glVertex2f (x + width, y + height);
    glVertex2f (x, y + height);
    glEnd();

    glDisable (GL_BLEND);

    // Restaura las matrices de proyección y modelo
    glPopMatrix();
    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);

    // Restaura el estado
    glPopAttrib();
}

void printText (const char *text, float x, float y, float r, float g, float b) {
    glPushAttrib (GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);  // Guarda el estado actual
    glDisable (GL_LIGHTING);
    glDisable (GL_DEPTH_TEST);  // Desactiva la prueba de profundidad para elementos 2D

    // Cambia a proyección ortográfica
    glMatrixMode (GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D (0, WINX, 0, WINY);
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Establece el color y la posición del texto
    glColor3f (r, g, b);
    glRasterPos2f (x, y);

    // Dibuja el texto
    for (int i = 0; text[i] != '\0'; i++) {
        glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    // Restaura las matrices anteriores
    glPopMatrix();
    glMatrixMode (GL_PROJECTION);
    glPopMatrix();
    glMatrixMode (GL_MODELVIEW);

    // Restaura el estado
    glPopAttrib();
}

void printMenu() {
    // Dibuja el fondo semi-transparente
    drawBox (WINX / 2 - 200, WINY / 2 - 100, 400, 200, 0.1f, 0.1f, 0.3f, 0.7f);

    // Luego dibuja el texto sobre el cuadro
    printText ("PANG 2025", WINX / 2 - 40, WINY / 2 + 60, 1.0f, 1.0f, 0.0f);
    printText ("Press SPACE to start", WINX / 2 - 80, WINY / 2 + 10, 0.0f, 1.0f, 0.0f);
    printText ("CONTROLS:", WINX / 2 - 50, WINY / 2 - 30, 1.0f, 1.0f, 1.0f);
    printText ("Arrow Keys: Move", WINX / 2 - 80, WINY / 2 - 60, 0.8f, 0.8f, 1.0f);
    printText ("SPACE: Shoot", WINX / 2 - 60, WINY / 2 - 90, 0.8f, 0.8f, 1.0f);
}

void printGameOver() {
    // Dibuja el fondo semi-transparente
    drawBox (WINX / 2 - 200, WINY / 2 - 70, 400, 160, 0.3f, 0.1f, 0.1f, 0.7f);

    // Luego dibuja el texto sobre el cuadro
    printText ("GAME OVER", WINX / 2 - 60, WINY / 2 + 60, 1.0f, 0.2f, 0.2f);

    char scoreText[50];
    sprintf (scoreText, "Final Score: %d", score);
    printText (scoreText, WINX / 2 - 70, WINY / 2, 1.0f, 1.0f, 1.0f);

    char highScoreText[50];
    sprintf (highScoreText, "High Score: %d", highScore);
    printText (highScoreText, WINX / 2 - 70, WINY / 2 - 30, 1.0f, 1.0f, 1.0f);

    printText ("Press SPACE to restart", WINX / 2 - 100, WINY / 2 - 60, 0.0f, 1.0f, 0.0f);
}

// Obtiene la puntuación más alta de un archivo
int getHighScore() {
    FILE *file = fopen ("scores.txt", "r");
    int highScore = 0;  // Variable para almacenar la puntuación más alta
    if (file) {
        int score;
        char date_str[100];
        char time_str[100];
        while (fscanf (file, "%99s %99s - Score: %d\n", date_str, time_str, &score) == 3) {
            if (score > highScore) {
                highScore =
                    score;  // Actualiza la puntuación más alta si la puntuación actual es mayor
            }
        }
        fclose (file);
    } else {
        printf ("Error al abrir scores.txt para lectura\n");
    }
    return highScore;
}

// Guarda la puntuación actual en un archivo
void saveScore() {
    FILE *file = fopen ("scores.txt", "a");
    if (!file) {
        printf ("Error al abrir scores.txt para escritura\n");
        return;
    }
    time_t now = time (NULL);
    struct tm *t = localtime (&now);

    char time_str[100];
    strftime (time_str, sizeof (time_str), "%Y-%m-%d %H:%M:%S", t);
    // Escribe la fecha y hora junto con la puntuación en el archivo
    fprintf (file, "%s - Score: %d\n", time_str, score);
    fclose (file);
}

// Logica del juego: mueve los objeto mandando el mensaje "move"
void myLogic() {
    // Solo ejecuta si el juego está en estado GAME_STATE
    if (gameState != GAME_STATE) return;

    int res;  // variable auxiliar que contiene el resultado de la comprobacion de colision

    if (theBullet) {
        float bulletPos[3];
        theBullet->getPos (bulletPos);
        if (bulletPos[Y] >= SIZEY2) {
            worldobjects.remove (theBullet);  // borra el proyectil
            theBullet = NULL;                 // esto quizás sobra pero por si acaso...
            shotTime = 0;                     // resetea el tiempo de vida del proyectil
        }
    }

    // Generar animales aleatorios

    if (!theAnimal && RAND_DOM (0, 100) == 1) {  // 1% de probabilidad de generar un animal
        theAnimal = new Animal();                // Crea un nuevo animal
        worldobjects.add (theAnimal);            // Añade el animal a la lista de objetos
    }

    if (theAnimal) {
        float animalPos[3] = {0, 0, 0};  // Posición del animal
        theAnimal->getPos (animalPos);   // Obtiene la posición del animal
        if (animalPos[Y] <= -SIZEY2) {
            worldobjects.remove (theAnimal);  // Si el animal está fuera de la pantalla, lo elimina
            theAnimal = NULL;                 // Resetea el puntero al animal
        }
    }

    // Pide al mundo que mueve los objetos
    worldobjects.move();

    // Comprueba si ha habido colisión, pasa referencia a proyectil y jugador, retorna tipo de
    // colisión res==0:  No ha colisicón res==1:  Bola/jugador res>=2:  Bola/Proyectil, depende del
    // tipo de bola (grande/mediano/pequeño)
    res = worldobjects.collisions (theBullet, theMan, theAnimal);
    // if (res) printf ("Collision result: %d\n", res);

    if (res == 1) {  // Bola/jugador
        if (nMan > 0) {
            AudioPlayer::playSound ("sounds/hit.wav");  // Reproduce el sonido de golpe
        }
        nMan--;

        // Si el jugador se queda sin vidas:
        if (nMan == 0) {
            if (gameState != GAMEOVER_STATE) {
                saveScore();              // Guarda la puntuación final
                if (score > highScore) {  // Si la puntuación es más alta
                    highScore = score;    // Actualiza la puntuación más alta
                }
                AudioPlayer::stopMusic();                         // Detiene la música
                AudioPlayer::playSound ("sounds/game_over.wav");  // Reproduce Game Over
            }
            gameState = GAMEOVER_STATE;  // Si no quedan naves, cambia al estado GAMEOVER
        } else {
            theMan->resetpos();                // recoloca el jugador al centro
            worldobjects.reposition (theMan);  // comprueba bola y jugador no colisionan
        }
    }

    if (res == 2 || res == 3 || res == 4) {               // Bola/Proyectil
        worldobjects.remove (theBullet);                  // borra el proyectil
        AudioPlayer::playSound ("sounds/explosion.wav");  // Reproduce el sonido de explosión
        theBullet = NULL;                                 // esto quizás sobra pero por si acaso...
        shotTime = 0;                                     // resetea el tiempo de vida del proyectil
        score += 100 * (res - 1);                         // Actualiza puntuación
    }

    if (res == 5) {                                    // Animal/Proyectil
        worldobjects.remove (theBullet);               // borra el proyectil
        AudioPlayer::playSound ("sounds/animal.wav");  // Reproduce el sonido de animal
        theBullet = NULL;                              // esto quizás sobra pero por si acaso...
        shotTime = 0;                                  // resetea el tiempo de vida del proyectil
        score += 1000;                                 // Actualiza puntuación
        worldobjects.remove (theAnimal);               // borra el animal
        theAnimal = NULL;                              // resetea el puntero al animal
    }

    if (res == 6) {                       // Obstáculo/ Proyectil
        worldobjects.remove (theBullet);  // borra el proyectil
        theBullet = NULL;                 // esto quizás sobra pero por si acaso...
        shotTime = 0;                     // resetea el tiempo de vida del proyectil
    }

    // No es necesario hacer nada
    // if (res == 7) {  // Obstáculo/ Bola
    // }

    if (specialKeysHeld[GLUT_KEY_LEFT]) { theMan->strafe (-0.15, 0, 0); }
    if (specialKeysHeld[GLUT_KEY_RIGHT]) { theMan->strafe (0.15, 0, 0); }
}

/**************************************************************/

void OnDibuja (void) {
    // Borrado de la pantalla
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Dibuja la imagen del fondo
    displaytext (KLtexture);

    // GAME STATE
    if (gameState == GAME_STATE) {
        // Manda el mensaje "draw" al mundo
        // Este mandará el mensaje "draw" a todas las figuras
        worldobjects.draw();

        // imprime los datos (vidas, puntuación)
        printdata();
    } else if (gameState == MENU_STATE) {
        // Dibuja el menú
        printMenu();
    } else if (gameState == GAMEOVER_STATE) {
        // Dibuja el mensaje de Game Over
        printGameOver();
    }

    // Al final, cambiar el buffer y redibuja la pantalla
    glutSwapBuffers();
    glutPostRedisplay();
}

/**************************************************************/

void resetGame() {
    nMan = 5;          // Resetea el número de vidas
    score = 0;         // Resetea la puntuación
    shotTime = 0;      // Resetea el tiempo de vida del proyectil
    theBullet = NULL;  // Resetea el puntero al proyectil
    theAnimal = NULL;  // Resetea el puntero al animal

    // Elimina todos los objetos del mundo excepto el jugador
    worldobjects.removeAllExceptMan (theMan);

    // Reposiciona el jugador al centro
    theMan->resetpos();

    // Crea las bolas iniciales
    worldobjects.createBalls();

    // Crea los obstáculos iniciales
    worldobjects.createObstacles();

    // Reproduce la música de fondo
    AudioPlayer::playMusic ("sounds/music.wav");
}

void exit_game() {
    AudioPlayer::stopMusic();  // Detiene la música
    exit (1);                  // Sale del programa
}

/**************************************************************/

void OnKeyboardDown (unsigned char key, int x, int y) {
    keysHeld[key] = true;

    switch (key) {
        case 'q':
        case ESC: exit_game(); break;
        case ' ':
            if (gameState != GAME_STATE) {
                resetGame();
                gameState = GAME_STATE;
            } else if (!theBullet) {
                theBullet = theMan->fire();
                worldobjects.add (theBullet);
            }
            break;
    }
}

void OnKeyboardUp (unsigned char key, int x, int y) {
    keysHeld[key] = false;
}

void OnSpecKeyboardDown (int key, int x, int y) {
    specialKeysHeld[key] = true;
}

void OnSpecKeyboardUp (int key, int x, int y) {
    specialKeysHeld[key] = false;
}
