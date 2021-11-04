#include<SDL.h>
#include"obstacle.hpp"

Obstacle::Obstacle()
    : rect{ 0, 0, 0, 0 }, vel( 0 ), active( false ), render_tex( NULL ) {}

void Obstacle::move()
{
    if( active )
        {
        rect.x += vel;
        if( rect.x + rect.w < 0 )
        {
            incapacitate();
        }
    }
}

void Obstacle::render( SDL_Renderer* renderer )
{
    if( active )
    {
        ( *render_tex ).render( rect.x, rect.y, NULL, &rect );
    }
}

SDL_Rect& Obstacle::get_rect()
{
    return rect;
}

void Obstacle::incapacitate()
{
    rect.x = 0;
    rect.y = 0;
    rect.w = 0;
    rect.h = 0;
    vel = 0;
    active = false;
}

void Obstacle::resuscitate(int _x, int _y, int _w, int _h, int _vel)
{
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    vel = _vel;
    active = true;
}

bool Obstacle::is_active()
{
    return active;
}

void Obstacle::attach_sprite( Texture* tex_ref )
{
    render_tex = tex_ref;
}
