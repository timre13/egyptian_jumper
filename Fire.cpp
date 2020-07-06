#include "Fire.h"

Fire::Fire(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList)
    : Entity(x, y, w, h, renderer, imageLoader, 1, 2, wallList)
{
    m_textureName = "fire_0";
}

void Fire::update()
{
    this->Entity::update();

    --m_animationCountdown;

    if (m_animationCountdown == 0)
    {
        m_animationCountdown = 10;

        ++m_currentAnimationFrame;

        if (m_currentAnimationFrame > 4)
            m_currentAnimationFrame = 0;

        m_textureName = "fire_"+std::to_string(m_currentAnimationFrame);
    }
}
