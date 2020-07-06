#ifndef COIN_H_
#define COIN_H_

#include "Entity.h"

class Coin : public Entity
{
protected:
    int m_currentAnimationFrame{};
    int m_animationCountdown{2};

public:
    Coin(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, std::vector<Wall*> *wallList);

    virtual void update() override;
};


#endif /* COIN_H_ */
