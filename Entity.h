#ifndef ENTITY_H_
#define ENTITY_H_

#include "GameObject.h"
#include "Wall.h"

class Entity : public GameObject
{
protected:
    int     m_maxHealth;
    int     m_health;
    int     m_damage;

public:
    Entity(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader, int health, int damage, std::vector<Wall*> *wallList);

    virtual void renderHitbox(const Camera *camera) const override;
    virtual void update() override;

    virtual inline bool getIsDead() const           { return m_health <= 0;     }
    virtual inline void kill()                      { m_health = 0;             }

    virtual inline void damage(int amount)          { m_health -= amount;
                                              if (m_health < 0) m_health = 0; }
    virtual inline void addHealth(int amount)       { m_health += amount;       }
    virtual inline int getDamageAmount() const      { return m_damage;          }
    virtual inline int getHealth() const            { return m_health;          }

    virtual bool getIsOnGround() const;

    virtual void render(const Camera *camera) const override;
};



#endif /* ENTITY_H_ */
