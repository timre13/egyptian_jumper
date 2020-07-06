#include "Coin.h"
#include "Wall.h"
#include <string>
#include <vector>

Coin::Coin(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList)
    : Entity(x, y, w, h, renderer, imageLoader, 1, 1, wallList) // we use the damage as the number of points the coin gives
{
    m_textureName = "coin_0";
}

void Coin::update()
{
    this->Entity::update();

    --m_animationCountdown;

    if (m_animationCountdown == 0)
    {
        m_animationCountdown = 2;

        ++m_currentAnimationFrame;

        if (m_currentAnimationFrame > 25)
            m_currentAnimationFrame = 0;

        m_textureName = "coin_"+std::to_string(m_currentAnimationFrame);
    }

    if (getIsDead())
        m_damage = 0;
}
