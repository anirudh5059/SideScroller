#include<SDL.h>
#include"obstacle.hpp"

Obstacle::Obstacle( int _pos_x, int _pos_y, int _width, int _height )
    : rect{_pos_x, _pos_y, _width, _height} {}

void Obstacle::move()
{
    rect.x += VEL;
}

void Obstacle::render( SDL_Renderer* renderer )
{
    SDL_RenderDrawRect( renderer, &rect );
}

SDL_Rect& Obstacle::get_rect()
{
    return rect;
}
