#include<SDL.h>
#include"utility.hpp"
#include"entity.hpp"
#include"obstacle.hpp"

void draw_circle( SDL_Renderer* renderer, MDEntity* ent )
{
    Circle circ( ent->get_cen_x(), ent->get_cen_y(), ent->get_cur_rad() );
    draw_circle( renderer, &circ );
}

void draw_circle( SDL_Renderer* renderer, Circle* circ )
{
    // cs/sn are the x/y axis component of the circle beyond the center
    int cur_rad = circ->rad;
    int cs = cur_rad;
    int cen_x = circ->cen_x;
    int cen_y = circ->cen_y;
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
