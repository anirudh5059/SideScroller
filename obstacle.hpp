#include<SDL.h>

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__
class Obstacle
{
  public:
    static const int VEL = -1;
    Obstacle( int pos_x, int pos_y, int height, int width );
    void render( SDL_Renderer* renderer );
    void move();
    SDL_Rect& get_rect();
  private:
    SDL_Rect rect;
};
#endif
