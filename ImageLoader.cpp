#include "ImageLoader.h"
#include "Logger.h"

ImageLoader::ImageLoader(SDL_Window *window, SDL_Renderer *renderer, const std::string &basePath)
{
    m_windowPtr     = window;
    m_rendererPtr   = renderer;
    m_basePath      = basePath;

    SDL_Texture *fallbackTexture{SDL_CreateTexture(m_rendererPtr, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, 2, 2)};

    uint32_t *pixels{nullptr};
    int pitch{};
    SDL_LockTexture(fallbackTexture, nullptr, (void**)&pixels, &pitch);

    pixels[0] = 0x00ff0000;
    pixels[1] = 0x0000ff00;
    pixels[2] = 0x000000ff;

    SDL_UnlockTexture(fallbackTexture);

    m_images.insert({"null", fallbackTexture});
}

int ImageLoader::loadImage(const std::string &filename, const std::string &name)
{
    std::string fileExtension{filename.find_last_of(".") == filename.npos ? " " : filename.substr(filename.find_last_of(".")+1)};

    if (fileExtension.compare("bmp"))
        Logger::warning("File extension is not BMP");

    SDL_Surface *imageSurface{SDL_LoadBMP((fs::path{m_basePath} / fs::path{filename}).string().c_str())};

    if (!imageSurface)
    {
        Logger::error("Failed to load image: "+std::string(SDL_GetError()));

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading image", ("Failed to load image: "+std::string(SDL_GetError())).c_str(), m_windowPtr);

        return 1;
    }

    SDL_Texture *imageTexture{SDL_CreateTextureFromSurface(m_rendererPtr, imageSurface)};

    if (!imageTexture)
    {
        Logger::error("Failed to convert SDL_Surface to SDL_Texture: "+std::string(SDL_GetError()));

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading image", ("Failed to convert SDL_Surface to SDL_Texture: "+std::string(SDL_GetError())).c_str(), m_windowPtr);

        SDL_FreeSurface(imageSurface);
        return 2;
    }

    SDL_FreeSurface(imageSurface);

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

    m_images.insert({imageName, imageTexture});

    return 0;
}

SDL_Texture* ImageLoader::getImage(const std::string &name)
{
    try
    {
        return m_images.at(name);
    }
    catch (std::exception &e)
    {
        Logger::error("No such image: "+name);

        // If not found, return the fallback image.
        return getImage("null");
    }
}

ImageLoader::~ImageLoader()
{
    for (auto &pair : m_images)
        SDL_DestroyTexture(pair.second);
}
