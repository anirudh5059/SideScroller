#include<SDL.h>
#include<algorithm>
#include "entity.hpp"
#include "obstacle.hpp"
#include "utility.hpp"
#include "consts.hpp"

extern const int NEG_INF;

MDEntity::MDEntity( int _min_radius, int _max_radius, int _cen_x, int _cen_y,
                    int max_width, int max_height )
    : cen_x(_cen_x), cen_y(_cen_y), min_rad(_min_radius), max_rad(_max_radius),
      vel_x(0), vel_y(0), rad_inc_rate(0)
{
    srand( SDL_GetTicks() );
    int eff_max_rad = std::min( { max_rad, cen_x, max_width - cen_x, cen_y,
                                  max_height - cen_y } );
    cur_rad = min_rad + rand() % ( eff_max_rad - min_rad + 1 );
}

int MDEntity::get_min_rad()
{
    return min_rad;
}

int MDEntity::get_cen_x()
{
    return cen_x;
}

int MDEntity::get_cen_y()
{
    return cen_y;
}

int MDEntity::get_cur_rad()
{
    return cur_rad;
}

void MDEntity::render( std::shared_ptr<SDL_Renderer> renderer )
{
    draw_circle( renderer, *this );
}

void MDEntity::handle_event( SDL_Event& e )
{
    // Key pressed, not recording already pressed response.
    if( ( e.type == SDL_KEYDOWN ) && ( e.key.repeat == 0 ) )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_w: vel_y -= AX_VEL; break;
            case SDLK_s: vel_y += AX_VEL; break;
            case SDLK_a: vel_x -= AX_VEL; break;
            case SDLK_d: vel_x += AX_VEL; break;
            case SDLK_9: rad_inc_rate += RAD_VEL; break;
            case SDLK_0: rad_inc_rate -= RAD_VEL; break;
        }
    } 

    // Key released, not recording already released response.
    else if( ( e.type == SDL_KEYUP ) && ( e.key.repeat == 0 ) )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_w: vel_y += AX_VEL; break;
            case SDLK_s: vel_y -= AX_VEL; break;
            case SDLK_a: vel_x += AX_VEL; break;
            case SDLK_d: vel_x -= AX_VEL; break;
            case SDLK_9: rad_inc_rate -= RAD_VEL; break;
            case SDLK_0: rad_inc_rate += RAD_VEL; break;
        }
    } 
}

bool MDEntity::check_collision_pixie( Pixie& pixie )
{
    Circle circ = pixie.get_circ();
    long long dst = ( cen_x - circ.cen_x ) * ( cen_x - circ.cen_x )
                    + ( cen_y - circ.cen_y ) * ( cen_y - circ.cen_y );
    if( dst >  ( cur_rad + circ.rad ) * ( cur_rad + circ.rad ) )
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool MDEntity::check_collision_obstcl( Obstacle& obstacle )
{
    SDL_Rect rect = obstacle.get_rect();
    int clos_x;
    int clos_y;

    // Determine closest x coordinate

    // Obstacle rectangle is to the right of the entity circle center.
    if( cen_x < rect.x )
    {
        clos_x = rect.x;
    }
    // Obstacle rectangle is to the left of the entity circle center.
    else if( cen_x > rect.x + rect.w )
    {
        clos_x = rect.x + rect.w;
    }
    // Obstacle rectangle's x coordinate overlaps that of the entity circle
    // center.
    else
    {
        clos_x = cen_x;
    }

    // Determine closest y coordinate

    // Obstacle rectangle is above the entity circle center.
    if( cen_y < rect.y )
    {
        clos_y = rect.y;
    }
    // Obstacle rectangle is below the entity circle center.
    else if( cen_y > rect.y + rect.h )
    {
        clos_y = rect.y + rect.h;
    }
    // Obstacle rectangle's y coordinate overlaps that of the entity circle
    // center.
    else
    {
        clos_y = cen_y;
    }
    
    // Test wether the closest point on the obstacle rectangle collides with
    // the entity circle
    if( ( clos_x - cen_x ) * ( clos_x - cen_x )
        + ( clos_y - cen_y ) * ( clos_y - cen_y ) > cur_rad * cur_rad )
    {
        return false;
    }
    else
    {
        return true;
    }
}

int MDEntity::move( int max_width, int max_height, std::vector<Obstacle>& obstacles,
                    std::vector<Pixie>& pixies )
{
    cen_x += vel_x;
    // If a collision occurred, move back
    if( ( cen_x - cur_rad < 0 ) || ( cen_x + cur_rad > max_width ) )
    {
        cen_x -= vel_x;
    }

    cen_y += vel_y;
    // If a collision occurred, move back
    if( ( cen_y - cur_rad < 0 ) || ( cen_y + cur_rad > max_height ) )
    {
        cen_y -= vel_y;
    }

    cur_rad += rad_inc_rate;
    if ( ( cur_rad + rad_inc_rate > max_rad )
         || ( cur_rad + rad_inc_rate < 0 )
         || ( cen_x + cur_rad + rad_inc_rate > max_width )
         || ( cen_x - cur_rad - rad_inc_rate  < 0 )
         || ( cen_y + cur_rad + rad_inc_rate > max_height )
         || ( cen_y - cur_rad - rad_inc_rate  < 0 ) )
    {
        cur_rad -= rad_inc_rate;
    }

    // Check for collisions amongst all obstacles
    for( int i = 0; i < MAX_OBSTCLS; ++i )
    {
        if( obstacles[i].is_active() )
        {
            if( check_collision_obstcl( obstacles[i] ) )
            {
                return NEG_INF;
            }
        }
    }

    // Check for pixie collisions
    for( int i = 0; i < MAX_PIXIES; ++i )
    {
        if( pixies[i].is_active() )
        {
            if( check_collision_pixie( pixies[i] ) )
            {
                pixies[i].incapacitate();
                return 1;
            }
        }
    }

    return 0;
}

bool comp( int a, int b )
{
    return( a < b );
}

// void MDEntity::populate( int pos_x, int pos_y, int max_height, int max_width )
// {
//     cen_x = pos_x;
//     cen_y = pos_y;
//     srand( SDL_GetTicks() );
//     int eff_max_rad = std::min( { max_rad, cen_x, max_width - cen_x, cen_y,
//                                   max_height - cen_y } );
//     cur_rad = min_rad + rand() % ( eff_max_rad - min_rad + 1 );
//     // Debug
//     //printf( "The current radius chosen is %d\n", cur_rad );
// }
