#include<SDL.h>

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__
class Obstacle
{
  public:
    Obstacle();
    void render( SDL_Renderer* renderer );
    void move();
    SDL_Rect& get_rect();
    void incapacitate();
    void resuscitate( int _x, int _y, int _w, int _h, int _vel );
    bool is_active();
  private:
    SDL_Rect rect;
    int vel;
    bool active;
};
#endif
