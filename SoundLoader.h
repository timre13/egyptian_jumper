#ifndef SOUNDLOADER_H_
#define SOUNDLOADER_H_

#include <map>
#include <string>
#include <SDL2/SDL.h>

struct SoundObject
{
    Uint8           *buffer;
    SDL_AudioSpec   spec;
    Uint32          length;
};

/*
 * Loads, stores and frees sounds.
 */
class SoundLoader
{
private:
    std::string                         m_basePath;
    std::map<std::string, SoundObject*> m_sounds;

public:
    SoundLoader(const std::string &basePath);

    int loadSound(const std::string &filename, const std::string name="");

    SoundObject* getSound(const std::string &name);

    ~SoundLoader();
};

#endif /* SOUNDLOADER_H_ */
