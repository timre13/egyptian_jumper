#include "Player.h"

Player::Player(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader *imageLoader, int health, int damage, std::vector<Wall*> *wallList)
: Entity(x, y, w, h, renderer, imageLoader, health, damage, wallList)
{
    m_textureName = "player_right";
}

void Player::jump()
{
    if (!getIsOnGround())
        return;

    m_jumpForce = 30;
}

void Player::update()
{
    this->Entity::update();

    m_y -= m_jumpForce;

    if (m_jumpForce > 0)
        --m_jumpForce;

    if (m_lastHorizontalMovingDirection > 0)
        m_textureName = "player_right";
    else
        m_textureName = "player_left";

#ifdef COLLISION_DETECTION_DEBUG
        std::cout << "Is right side of player colliding? ";
        {
            bool isColliding{false};
            for (auto wall : wallList)
                if (player->getIsRightSideCollidingWith(wall))
                    isColliding = true;
            std::cout << (isColliding ? "yes" : "no") << '\n';
        }
        std::cout << "Is left side of player colliding? ";
        {
            bool isColliding{false};
            for (auto wall : wallList)
                if (player->getIsLeftSideCollidingWith(wall))
                    isColliding = true;
            std::cout << (isColliding ? "yes" : "no") << '\n';
        }
        std::cout << "Is top side of player colliding? ";
        {
            bool isColliding{false};
            for (auto wall : wallList)
                if (player->getIsTopSideCollidingWith(wall))
                    isColliding = true;
            std::cout << (isColliding ? "yes" : "no") << '\n';
        }
        std::cout << "Is bottom side of player colliding? ";
        {
            bool isColliding{false};
            for (auto wall : wallList)
                if (player->getIsBottomSideCollidingWith(wall))
                    isColliding = true;
            std::cout << (isColliding ? "yes" : "no") << '\n';
        }

        std::cout << '\n';
#endif
}


void Player::moveX(int plusX)
{
    this->GameObject::moveX(plusX);

    m_lastHorizontalMovingDirection = plusX;
}
