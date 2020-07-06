#ifndef WALL_H_
#define WALL_H_

#include "GameObject.h"
#include "ImageLoader.h"

class Wall : public GameObject
{
public:
    Wall(int x, int y, int w, int h, SDL_Renderer *renderer, ImageLoader* imageLoader);
};


#endif /* WALL_H_ */
