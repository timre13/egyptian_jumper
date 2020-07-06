#include "Entity.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

Entity::Entity(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader *imageLoader, int health, int damage, std::vector<Wall*> *wallList)
    : GameObject(x, y, w, h, renderer, imageLoader, wallList)
{
    m_health    = health;
    m_maxHealth = health;
    m_damage    = damage;
}

void Entity::renderHitbox(const Camera *camera) const
{
    if (
            m_x-camera->getX()+m_w < 0 ||
            m_x-camera->getX() > WINDOW_WIDTH ||
            m_y-camera->getY()+m_h < 0 ||
            m_y-camera->getY() > WINDOW_HEIGHT)
        return;

    SDL_Rect hitboxRect{m_x-camera->getX(), m_y-camera->getY(), m_w, m_h};

    SDL_SetRenderDrawColor(m_rendererPtr, 0, 255, 0, 255);
    SDL_RenderDrawRect(m_rendererPtr, &hitboxRect);
    SDL_RenderDrawLine(m_rendererPtr, m_x-camera->getX(), m_y-camera->getY(), m_x+m_w-camera->getX(), m_y+m_h-camera->getY());
    SDL_RenderDrawLine(m_rendererPtr, m_x-camera->getX(), m_y+m_h-camera->getY(), m_x+m_w-camera->getX(), m_y-camera->getY());

    SDL_Rect hpRect{m_x-camera->getX(), m_y-camera->getY(), (int)((double)m_health/m_maxHealth*m_w), 10};
    SDL_RenderFillRect(m_rendererPtr, &hpRect);
}

void Entity::update()
{
    if (getIsDead())
        m_damage = 0;

    if (!getIsOnGround())
        m_y += 8;

    if (m_y > 1500)
        damage(1);
}

bool Entity::getIsOnGround() const
{
    bool isColliding{false};
    for (int i{}; i < m_wallListPtr->size(); ++i)
    {
        Wall *wall{m_wallListPtr->operator [](i)};

        if (getIsBottomSideCollidingWith(wall))
        {
            isColliding = true;
            break;
        }
    }

    return isColliding;
}

void Entity::render(const Camera *camera) const
{
    if (
            m_x-camera->getX()+m_w < 0 ||
            m_x-camera->getX() > WINDOW_WIDTH ||
            m_y-camera->getY()+m_h < 0 ||
            m_y-camera->getY() > WINDOW_HEIGHT)
        return;

    if (getIsDead())
        return;

    this->GameObject::render(camera);
}
