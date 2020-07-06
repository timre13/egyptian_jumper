#include "GameObject.h"

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

GameObject::GameObject(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList)
{
    m_x                 = x;
    m_y                 = y;
    m_w                 = w;
    m_h                 = h;
    m_rendererPtr       = renderer;
    m_imageLoaderPtr    = imageLoader;
    m_wallListPtr       = wallList;
}

void GameObject::render(const Camera *camera) const
{
    if (
            m_x-camera->getX()+m_w < 0 ||
            m_x-camera->getX() > WINDOW_WIDTH ||
            m_y-camera->getY()+m_h < 0 ||
            m_y-camera->getY() > WINDOW_HEIGHT)
        return;

    SDL_Rect srect{0, 0, m_w, m_h};
    SDL_Rect drect{m_x-camera->getX(), m_y-camera->getY(), m_w, m_h};

    if (SDL_RenderCopy(m_rendererPtr, m_imageLoaderPtr->getImage(m_textureName), &srect, &drect))
    {
        Logger::error("SDL_RenderCopy() failed: "+std::string(SDL_GetError()));
    }
}

void GameObject::renderHitbox(const Camera *camera) const
{
    if (
            m_x-camera->getX()+m_w < 0 ||
            m_x-camera->getX() > WINDOW_WIDTH ||
            m_y-camera->getY()+m_h < 0 ||
            m_y-camera->getY() > WINDOW_HEIGHT)
        return;

    SDL_Rect hitboxRect{m_x-camera->getX(), m_y-camera->getY(), m_w, m_h};

    SDL_SetRenderDrawColor(m_rendererPtr, 255, 0, 0, 255);
    SDL_RenderDrawRect(m_rendererPtr, &hitboxRect);
    SDL_RenderDrawLine(m_rendererPtr, m_x-camera->getX(), m_y-camera->getY(), m_x+m_w-camera->getX(), m_y+m_h-camera->getY());
    SDL_RenderDrawLine(m_rendererPtr, m_x-camera->getX(), m_y+m_h-camera->getY(), m_x+m_w-camera->getX(), m_y-camera->getY());
}

bool GameObject::getIsColliding(GameObject *object) const
{
    int distanceTreshold{5};

    return  (std::abs(object->getX()-getRightX()) <= distanceTreshold &&
            object->getY() < getBottomY() &&
            object->getBottomY() > m_y) ||
            (std::abs(getX()-object->getRightX()) <= distanceTreshold &&
            m_y < object->getBottomY() &&
            getBottomY() > object->getY()) ||
            (std::abs(m_y-object->getBottomY()) <= distanceTreshold &&
            object->getRightX() > m_x &&
            object->getX() < getRightX()) ||
            (std::abs(object->getY()-getBottomY()) <= distanceTreshold &&
            getRightX() > object->getX() &&
            m_x < object->getRightX());
}

bool GameObject::getIsRightSideCollidingWith(GameObject *object) const
{
    return  std::abs(object->getX()-getRightX()) <= 4 &&
            object->getY() < getBottomY() &&
            object->getBottomY() > m_y;
}

bool GameObject::getIsLeftSideCollidingWith(GameObject *object) const
{
    return  std::abs(getX()-object->getRightX()) <= 4 &&
            m_y < object->getBottomY() &&
            getBottomY() > object->getY();
}

bool GameObject::getIsTopSideCollidingWith(GameObject *object) const
{
    return  std::abs(m_y-object->getBottomY()) <= 4 &&
            object->getRightX() > m_x &&
            object->getX() < getRightX();
}

bool GameObject::getIsBottomSideCollidingWith(GameObject *object) const
{
    return  std::abs(object->getY()-getBottomY()) <= 4 &&
            getRightX() > object->getX() &&
            m_x < object->getRightX();
}

void GameObject::moveX(int plusX)
{
    for (int i{}; i < std::abs(plusX); ++i)
    {
        bool isColliding{false};

        for (int i{}; i < m_wallListPtr->size(); ++i)
        {
            Wall *wall{m_wallListPtr->operator[](i)};

            if (((plusX > 0) ? getIsRightSideCollidingWith(wall) : getIsLeftSideCollidingWith(wall)))
            {
                isColliding = true;
                break;
            }
        }

        if (isColliding)
            return;

        m_x += (plusX > 0 ? 1 : -1);
    }
}

void GameObject::moveY(int plusY)
{
    for (int i{}; i < std::abs(plusY); ++i)
    {
        bool isColliding{false};

        for (int i{}; i < m_wallListPtr->size(); ++i)
        {
            Wall *wall{m_wallListPtr->operator[](i)};

            if (((plusY > 0) ? getIsBottomSideCollidingWith(wall) : getIsTopSideCollidingWith(wall)))
            {
                isColliding = true;
                break;
            }
        }

        if (isColliding)
            return;

        m_y += (plusY > 0 ? 1 : -1);
    }
}
