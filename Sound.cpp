#include "Sound.h"
#include "Logger.h"
#include <SDL2/SDL.h>

namespace Sound
{

int deviceID = -1;

void init()
{
    SDL_InitSubSystem(SDL_INIT_AUDIO);
}

void play(SoundObject *soundObject)
{
    if (!SDL_WasInit(SDL_INIT_AUDIO))
        Logger::fatal("Sound::init() has not been called");

    // If we has not opened an audio device, open one
    if (deviceID == -1)
        deviceID = SDL_OpenAudioDevice(nullptr, false, &soundObject->spec, nullptr, false);

    if (deviceID <= 2 && deviceID != -1)
        Logger::error("Failed to open audio device: "+std::string(SDL_GetError()));

    SDL_ClearQueuedAudio(deviceID);

    if (SDL_QueueAudio(deviceID, soundObject->buffer, soundObject->length))
        Logger::error("SDL_QueueAudio() failed: "+std::string(SDL_GetError()));

    SDL_PauseAudioDevice(deviceID, false);
}

void quit()
{
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

}
