#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include "Wall.h"
#include <vector>

class Player final : public Entity
{
private:
    int m_lastHorizontalMovingDirection{1};
    int m_score{};
    int m_jumpForce{};

public:
    Player(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader *imageLoader, int health, int damage, std::vector<Wall*> *wallList);

    virtual void update() override;
    virtual void moveX(int plusX) override;

    virtual void jump();

    virtual inline int getScore() const { return m_score; }
    virtual inline void addScore(int amount) { m_score += amount; }
};

#endif /* PLAYER_H_ */
