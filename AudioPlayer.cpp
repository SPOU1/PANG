#include "AudioPlayer.h"
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

pid_t AudioPlayer::musicPid = 0;  // Inicializa el PID de la música a 0

void AudioPlayer::playSound (const char *filename) {
    pid_t pid = fork();  // Crea un nuevo proceso hijo
    if (pid == 0) {
        setsid();  // Crea una nueva sesión para el proceso hijo
        execlp ("aplay", "aplay", "-q", filename, (char *) NULL);

        // Si execlp falla, imprime un mensaje de error y termina el proceso hijo
        perror ("Error al ejecutar aplay");
        _exit (1);
    }
}

void AudioPlayer::playMusic (const char *filename) {
    if (musicPid == 0) {
        musicPid = fork();
        if (musicPid == 0) {
            // Proceso hijo para reproducir música
            setsid();

            // Redirige la salida para suprimir errores
            int devNull = open ("/dev/null", O_WRONLY);
            if (devNull >= 0) {
                dup2 (devNull, STDOUT_FILENO);
                dup2 (devNull, STDERR_FILENO);
                close (devNull);
            }

            // Usa un bucle infinito para la reproducción continua
            while (true) {
                // Usa fork para aislar cada reproducción
                pid_t audio_pid = fork();
                if (audio_pid == 0) {
                    execlp ("aplay", "aplay", "-q", filename, NULL);
                    _exit (0);
                }

                // Espera a que esta instancia termine
                int status;
                waitpid (audio_pid, &status, 0);

                // Agrega un pequeño retraso antes de reiniciar
                usleep (10000);  // 10ms
            }
            _exit (0);
        }
    }
}

void AudioPlayer::stopMusic() {
    if (musicPid > 0) {
        // Termina todo el grupo de procesos
        kill (-musicPid, SIGTERM);
        waitpid (musicPid, NULL, 0);
        musicPid = 0;
    }
}