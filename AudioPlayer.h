#include "commonstuff.h"
#include "GLstuff.h"
#include <unistd.h>
#include <sys/wait.h>

#ifndef __AUDIOPLAYER_H__
    #define __AUDIOPLAYER_H__
class AudioPlayer {
   private:
    static pid_t musicPid;

   public:
    static void playSound (const char *filename);
    static void playMusic (const char *filename);
    static void stopMusic();
};

#endif