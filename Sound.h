#ifndef SOUND_H_
#define SOUND_H_

#include "SoundLoader.h"

#include <string>
#include <SDL2/SDL.h>

namespace Sound
{
    extern int deviceID;

    void init();

    void play(SoundObject *soundObject);

    void quit();
};



#endif /* SOUND_H_ */
