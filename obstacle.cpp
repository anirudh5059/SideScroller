#include<SDL.h>
#include<stdexcept>
#include"obstacle.hpp"
#include"utility.hpp"
#include "consts.hpp"

TDEntity::TDEntity( std::shared_ptr<SDL_Renderer> renderer) :  vel_x( 0 ),
    vel_y( 0 ), active( false ), sprite( renderer ) {}


bool TDEntity::is_active()
{
    return active;
}

// void TDEntity::attach_sprite( Texture* tex_ref )
// {
//     render_tex = tex_ref;
// }

void TDEntity::incapacitate() {}

void TDEntity::resuscitate() {}

Obstacle::Obstacle( std::shared_ptr<SDL_Renderer> renderer ) : TDEntity( renderer ),
    rect{ 0, 0, 0, 0 } {}

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
    vel_x = 0;
    vel_y = 0;
    active = false;
}

void Obstacle::resuscitate( int _x, int _y, int _w, int _h, int _vel_x,
                            int _vel_y )
{
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
    vel_x = _vel_x;
    vel_y = _vel_y;
    active = true;
}


void Obstacle::move( int max_width, int max_height )
{
    if( active )
        {
        rect.x += vel_x;
        if( rect.x + rect.w < 0 || rect.x > max_width )
        {
            incapacitate();
        }

        rect.y += vel_y;
        if( rect.y + rect.h < 0 || rect.h > max_height )
        {
            incapacitate();
        }
    }
}

void Obstacle::render()
{
    if( active )
    {
        if( sprite.texture )
        {
            sprite.render( rect.x, rect.y, NULL, &rect );
        }
        else
        {
            throw std::runtime_error( "rendering texture not set for obstacle" );
        }
    }
}

Circle::Circle( int _cen_x, int _cen_y, int _rad ) : cen_x( _cen_x ),
        cen_y( _cen_y ), rad( _rad ) {}

Pixie::Pixie( std::shared_ptr<SDL_Renderer> renderer ): TDEntity( renderer ),
    circ{ 0, 0, 0 } {}

void Pixie::incapacitate()
{
    circ.cen_x = 0;
    circ.cen_y = 0;
    circ.rad = 0;
    vel_x = 0;
    vel_y = 0;
    active = false;
}

void Pixie::resuscitate( int _cen_x, int _cen_y, int _rad, int _vel_x,
                         int _vel_y )
{
    circ.cen_x = _cen_x;
    circ.cen_y = _cen_y;
    circ.rad = _rad;
    vel_x = _vel_x;
    vel_y = _vel_y;
    active = true;
}

void Pixie::move( int max_width, int max_height )
{
    if( active )
    {
        circ.cen_x += vel_x;
        if( circ.cen_x + circ.rad < 0 || circ.cen_x - circ.rad > max_width )
        {
            incapacitate();
        }
        circ.cen_y += vel_y;
        if( circ.cen_y + circ.rad < 0 || circ.cen_y - circ.rad > max_height )
        {
            incapacitate();
        }
    }
}

void Pixie::render()
{
    if( active )
        {
        SDL_Rect pix_rect = { 0, 0, 2 * PIXIE_RAD, 2 * PIXIE_RAD };
        if( sprite.texture )
        {
            sprite.render( circ.cen_x - circ.rad, circ.cen_y - circ.rad,
                                    NULL, &pix_rect );
        }
        else
        {
            throw( "rendering texture not set for obstacle" );
        }
    }
}

Circle& Pixie::get_circ()
{
    return circ;
}
