#include<SDL.h>
#include"texture.hpp"

#ifndef __OBSTACLE_H__
#define __OBSTACLE_H__

class TDEntity
{
  public:
    TDEntity( std::shared_ptr<SDL_Renderer> renderer );
    void render();
    virtual void move( int max_width, int max_height ) = 0;
    void incapacitate();
    void resuscitate();
    bool is_active();
    ObjectTexture sprite; 
    // void attach_sprite( Texture* sprite );

  protected:
    int vel_x;
    int vel_y;
    bool active;
};

// Obstacle of rectangular shape
class Obstacle : public TDEntity
{
  public:
    Obstacle( std::shared_ptr<SDL_Renderer> renderer );
    void render();
    void move( int max_width, int max_height );
    void incapacitate();
    void resuscitate( int _x, int _y, int _w, int _h, int _vel_x, int vel_y );
    SDL_Rect& get_rect();
    SDL_Rect rect;

  protected:
};

// Generic Circular shape
class Circle
{
  public:
      Circle( int _cen_x, int _cen_y, int _rad );
      int cen_x;
      int cen_y;
      int rad;
};

class Pixie : public TDEntity
{
  public:
    Pixie( std::shared_ptr<SDL_Renderer> renderer );
    void render();
    void move( int max_width, int max_height );
    void incapacitate();
    void resuscitate( int _cen_x, int _cen_y, int _rad, int _vel_x,
                      int _vel_y );
    Circle& get_circ();

  protected:
    Circle circ;
};
#endif
