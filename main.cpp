#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<iostream>
#include<string>
#include<vector>
#include<stdexcept>
#include "entity.hpp"
#include "obstacle.hpp"
#include "texture.hpp"
#include "consts.hpp"
#include "constructs.hpp"

int main()
{
    GameConstructs constructs;

    try
    {
        constructs.init( true, true, "images/background.png",
                "images/column_sprite.png", "images/pix_sprite.png",
                "fonts/ARCADECLASSIC.ttf" ); 
    }
    catch ( std::runtime_error& exception )
    {
        std::cout<<"Essential SDL initialization failed! Exiting!";
        throw;
    }

    SDL_Event event;
    Uint32 start_ticks = 0;
    int interval_count = 0;
    srand(SDL_GetTicks());
    bool quit = false;

    while( !quit )
    {
        while( SDL_PollEvent( &event ) != 0 )
        {
            if( event.type == SDL_QUIT )
            {
                quit = true;
            }

            constructs.ent.handle_event( event );
        }
        
        // Timer to perform obstacle addition every INTERVAL milliseconds.
        if( SDL_GetTicks() - start_ticks > INTERVAL )
        {
            interval_count++;
            bool obstcl_ping = false;
            if( interval_count > 14 )
            {
                obstcl_ping = true;
                interval_count = 0;
            }
            constructs.generate_environment( obstcl_ping );
            start_ticks = SDL_GetTicks();
        }

        constructs.move_environment();

        try
        {
            constructs.move_entity();
            constructs.render();
        }
        catch( ... )
        {
            throw;
        }

    }
}
