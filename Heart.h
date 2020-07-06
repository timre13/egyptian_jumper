#ifndef HEART_H_
#define HEART_H_

#include "Entity.h"

class Heart : public Entity
{
protected:
    int m_animationCountdown{30};
    int m_currentAnimationFrame{0};

public:
    Heart(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList);

    virtual void update() override;
};



#endif /* HEART_H_ */
