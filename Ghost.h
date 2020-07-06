#ifndef GHOST_H_
#define GHOST_H_

#include "Entity.h"

class Ghost : public Entity
{
private:
    int m_lastHorizontalMovingDirection{1};
    int m_lastVerticalMovingDirection{1};

public:
    Ghost(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader *imageLoader, int health, int damage, std::vector<Wall*> *wallList);

    virtual void update() override;
};



#endif /* GHOST_H_ */
