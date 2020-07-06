#ifndef FIRE_H_
#define FIRE_H_

#include "Entity.h"

class Fire : public Entity
{
private:
    int m_animationCountdown{10};
    int m_currentAnimationFrame{0};

public:
    Fire(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList);

    virtual void update() override;
};


#endif /* FIRE_H_ */
