#include "Ghost.h"
#include <cstdlib>

Ghost::Ghost(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader *imageLoader, int health, int damage, std::vector<Wall*> *wallList)
    : Entity(x, y, w, h, renderer, imageLoader, health, damage, wallList)
{
    m_textureName = "ghost_right";
}

void Ghost::update()
{
    if (std::rand()%100+1 > 99)
        m_lastHorizontalMovingDirection *= -1;
    if (std::rand()%100+1 > 97)
        m_lastVerticalMovingDirection *= -1;

    setX(getX() + m_lastHorizontalMovingDirection);
    setY(getY() + m_lastVerticalMovingDirection);

    if (m_lastHorizontalMovingDirection > 0)
        m_textureName = "ghost_right";
    else
        m_textureName = "ghost_left";
}
