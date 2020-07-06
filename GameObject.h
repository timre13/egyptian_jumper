#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "Logger.h"
#include "ImageLoader.h"
#include "Camera.h"

class Wall;

class GameObject
{
protected:
    int             m_x;
    int             m_y;
    int             m_w;
    int             m_h;
    SDL_Renderer    *m_rendererPtr;
    ImageLoader     *m_imageLoaderPtr;
    std::string     m_textureName{"null"};
    std::vector<Wall*> *m_wallListPtr;

public:
    GameObject(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList);

    virtual inline void update() {};
    virtual void render(const Camera *camera) const;
    virtual void renderHitbox(const Camera *camera) const;

    virtual inline int getX() const { return m_x; }
    virtual inline int getY() const { return m_y; }
    virtual inline int getW() const { return m_w; }
    virtual inline int getH() const { return m_h; }
    virtual inline int getRightX()  const { return m_x+m_w; }
    virtual inline int getBottomY() const { return m_y+m_h; }

    virtual inline void setX(int newX) { m_x = newX; }
    virtual inline void setY(int newY) { m_y = newY; }
    virtual inline void setW(int newW) { m_w = newW; }
    virtual inline void setH(int newH) { m_h = newH; }

    // get if the x side of **this** is colliding with *object*
    virtual bool getIsRightSideCollidingWith(GameObject *object) const;
    virtual bool getIsLeftSideCollidingWith(GameObject *object) const;
    virtual bool getIsTopSideCollidingWith(GameObject *object) const;
    virtual bool getIsBottomSideCollidingWith(GameObject *object) const;
    virtual bool getIsColliding(GameObject *object) const;

    virtual void moveX(int plusX);
    virtual void moveY(int plusY);

    virtual inline ~GameObject() {};
};

// This fixes a circular dependency, WTF?
#include "Wall.h"

#endif /* GAMEOBJECT_H_ */
