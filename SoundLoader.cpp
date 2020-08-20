#include "SoundLoader.h"
#include "Logger.h"

#include <filesystem>

namespace fs = std::filesystem;

SoundLoader::SoundLoader(const std::string &basePath)
{
    m_basePath = basePath;
}

int SoundLoader::loadSound(const std::string &filename, const std::string name)
{
    std::string fileExtension{filename.find_last_of(".") == filename.npos ? " " : filename.substr(filename.find_last_of(".")+1)};

    if (fileExtension.compare("wav"))
        Logger::warning("File extension is not WAV");

    std::string imageName = name;

    if (imageName.empty())
    {

        std::string::size_type afterSeparators{(filename.find_last_of(fs::path::preferred_separator) == filename.npos ?
                0 :
                (filename.find_last_of(fs::path::preferred_separator)+1))};

        imageName = filename.substr(
                afterSeparators,
                (filename.find_last_of(".")-afterSeparators));
    }

    auto soundObject = new SoundObject;

    if (!SDL_LoadWAV((fs::path{m_basePath} / fs::path{filename}).string().c_str(), &soundObject->spec, &soundObject->buffer, &soundObject->length))
    {
        Logger::error("Failed to load audio file: "+filename);
        return 1;
    }

    m_sounds.insert({imageName, soundObject});

    return 0;
}

SoundObject* SoundLoader::getSound(const std::string &name)
{
    try
    {
        return m_sounds.at(name);
    }
    catch (std::exception &e)
    {
        Logger::error("No such sound: "+name);

        return nullptr;
    }
}

SoundLoader::~SoundLoader()
{
    for (auto pair: m_sounds)
        SDL_FreeWAV(pair.second->buffer);
}

