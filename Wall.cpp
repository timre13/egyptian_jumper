#include "Wall.h"

Wall::Wall(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader)
    : GameObject(x, y, w, h, renderer, imageLoader, nullptr)
{
    m_textureName = "wall";
}
