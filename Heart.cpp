#include "Heart.h"

Heart::Heart(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList)
    : Entity(x, y, w, h, renderer, imageLoader, 1, 10, wallList)
{
    m_textureName = "heart_0";
}

void Heart::update()
{
    this->Entity::update();

    --m_animationCountdown;

    if (m_animationCountdown == 0)
    {
        m_animationCountdown = 30;

        ++m_currentAnimationFrame;

        if (m_currentAnimationFrame > 1)
            m_currentAnimationFrame = 0;

        m_textureName = "heart_"+std::to_string(m_currentAnimationFrame);
    }

    if (getIsDead())
        m_damage = 0;
}
