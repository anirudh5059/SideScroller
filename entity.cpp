#include<SDL.h>
#include<cstdlib>
#include"entity.hpp"
#include"obstacle.hpp"

MDEntity::MDEntity( int _x, int _y, int _min_radius, int _max_radius ) 
    : cen_x(_x), cen_y(_y), min_rad(_min_radius), max_rad(_max_radius),
      vel_x(0), vel_y(0)
{
    srand( SDL_GetTicks() );
    cur_rad = min_rad + rand() % ( max_rad - min_rad + 1 );
    // Debug
    //printf( "The current radius chosen is %d\n", cur_rad );
}

int MDEntity::get_min_rad()
{
    return min_rad;
}

void MDEntity::inc_rad()
{
    if ( cur_rad + 2 <= max_rad )
    {
        cur_rad += 2;
    }
}

void MDEntity::dec_rad()
{
    if ( cur_rad - 2 >= min_rad )
    {
        cur_rad -= 2;
    }
}

void MDEntity::render( SDL_Renderer* renderer )
{
    // cs/sn are the x/y axis component of the circle beyond the center
    int cs = cur_rad;
    int sn = 0;
    // Previous values of the above for recursive computation
    int cs_prev;
    int sn_prev;

    while ( cs >= sn )
    {
        // Render one half quadrant, then replicate across all 8 half quadrants
        SDL_RenderDrawPoint( renderer, cen_x + cs, cen_y + sn );
        SDL_RenderDrawPoint( renderer, cen_x + cs, cen_y - sn );
        SDL_RenderDrawPoint( renderer, cen_x - cs, cen_y + sn );
        SDL_RenderDrawPoint( renderer, cen_x - cs, cen_y - sn );
        SDL_RenderDrawPoint( renderer, cen_x + sn, cen_y + cs );
        SDL_RenderDrawPoint( renderer, cen_x + sn, cen_y - cs );
        SDL_RenderDrawPoint( renderer, cen_x - sn, cen_y + cs );
        SDL_RenderDrawPoint( renderer, cen_x - sn, cen_y - cs );

        // Upate the previous values
        cs_prev = cs;
        sn_prev = sn;

        // Update the values of cs and sn using midpoint algorithm
        // TODO: Make a doc
        sn = sn_prev + 1;
        if( (float)(cs_prev - 0.5)*(float)(cs_prev - 0.5) + sn_prev*sn_prev
            - cur_rad*cur_rad > 0 )
        {
            cs = cs_prev - 1;
        }
    }
}

void MDEntity::handle_event( SDL_Event& e )
{
    // Key pressed, not recording already pressed response.
    if( e.type == SDL_KEYDOWN )
    {
        if ( e.key.repeat == 0 )
        {
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: vel_y -= AX_VEL; break;
                case SDLK_DOWN: vel_y += AX_VEL; break;
                case SDLK_LEFT: vel_x -= AX_VEL; break;
                case SDLK_RIGHT: vel_x += AX_VEL; break;
            }
        }
        else
        {
            switch( e.key.keysym.sym )
            {
                case SDLK_q: inc_rad(); break;
                case SDLK_w: dec_rad(); break;
            }
        }
    } 

    // Key released, not recording already released response.
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: vel_y += AX_VEL; break;
            case SDLK_DOWN: vel_y -= AX_VEL; break;
            case SDLK_LEFT: vel_x += AX_VEL; break;
            case SDLK_RIGHT: vel_x -= AX_VEL; break;
        }
    } 
}

bool MDEntity::check_collision( SDL_Rect& rect )
{
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

bool MDEntity::move( int max_width, int max_height, Obstacle obstacles[],
                     int num_obstcls )
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

    // Check for collisions amongst all obstacles
    for( int i = 0; i < num_obstcls; ++i )
    {
        if( obstacles[i].is_active() )
        {
            if( check_collision( obstacles[i].get_rect() ) )
            {
                return false;
            }
        }
    }

    return true;
}
