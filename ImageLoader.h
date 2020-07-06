#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <SDL2/SDL.h>
#include <map>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

/*
 * Handles the loading and freeing of the images,
 * so the GameObject instanced don't have to do these operations themselves.
 */
class ImageLoader final
{
private:
    SDL_Window                              *m_windowPtr;
    SDL_Renderer                            *m_rendererPtr;
    std::map<std::string, SDL_Texture*>     m_images;
    std::string                             m_basePath;

public:

    /*
     * Initializes an ImageLoader object.
     * "window" is your main window.
     * "renderer" is an SDL_Renderer where you want to render the images.
     * "basepath" is the current working directory.
     */
    ImageLoader(SDL_Window *window, SDL_Renderer *renderer, const std::string &basePath);

    /*
     * Loads and image. Stores it as the specified name or if it is empty, uses the filename without the path and extension.
     * You can get the loaded image by calling getImage(). The images are free'd automatically by the destructor.
     *
     * Returns: 0 if successfully loaded the image,
     * 1 if failed to load the image
     * and 2 if failed to convert the SDL_Surface object to SDL_Texture.
     *
     * Also logs to stderr on error and shows a messagebox.
     */
    int loadImage(const std::string &filename, const std::string &name="");

    /*
     * Returns a pointer to the requested image.
     * Returns a generated image, named "null" if the image was not found.
     */
    SDL_Texture* getImage(const std::string &name);

    /*
     * Frees the images.
     */
    ~ImageLoader();
};

#endif /* IMAGELOADER_H_ */
