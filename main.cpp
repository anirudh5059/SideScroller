#include<SDL.h>
#include<SDL_image.h>
#include<string>
#include<vector>
#include"entity.hpp"
#include"obstacle.hpp"

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

SDL_Window* glob_win;
SDL_Renderer* glob_renderer;
const int INTERVAL = 4000;
const int MAX_OBSTCLS = 5;

bool init( bool png_load=false )
{
    bool init = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Error initializing SDL: %s\n ", SDL_GetError() );
        init = false;
    }
    else
    {
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Could not enable linear texture filtering, continuing\n" );
        }
        
        glob_win = SDL_CreateWindow( "PetProj", SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                     SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( glob_win == NULL )
        {
            printf( "Window could not be created, SDL Error: %s\n",
                    SDL_GetError() );
            init = false;
        }
        else
        {
            glob_renderer = SDL_CreateRenderer( glob_win, -1,
                                                ( SDL_RENDERER_ACCELERATED
                                                  | SDL_RENDERER_PRESENTVSYNC )
                                              );
            if( glob_renderer == NULL )
            {
                printf( "Renderer could not be created, SDL Error: %s\n",
                        SDL_GetError );
                init = false;
            }
            else
            {
                SDL_SetRenderDrawColor( glob_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                // Load the SDL_Image functionality if required
                if( png_load )
                {
                   int img_flags = IMG_INIT_PNG;  
                   if( !(IMG_Init( img_flags ) & img_flags ) )
                   {
                       printf( "SDL_Image could not be loaded, SDL Error: %s\n",
                               SDL_GetError() );
                       init = false;
                   }
                }
            }
        }
    }    
    return init;
}

int main()
{
    if ( !init() )
    {
        printf( "Could not initialize, exiting\n" );
        return (-1);
    }

    MDEntity ent( 100, 100, 20, 100 );
    std::vector< Obstacle > obstacles;
    obstacles.push_back( Obstacle ( 600, 10, 20, 250 ) );
    //Obstacle obstcl( 600, 10, 20, 250 );
    SDL_Event e;
    Uint32 start_ticks = 0;
    srand(SDL_GetTicks());
    bool quit = false;
    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }

            ent.handle_event( e );
        }
        
        // Timer to perform obstacle addition every INTERVAL milliseconds.
        if( SDL_GetTicks() - start_ticks > INTERVAL )
        {
            int obstcl_height = 100 + ( rand() % ( SCREEN_HEIGHT
                                                   -100 
                                                   - 5 * ent.get_min_rad()
                                                   + 1 ) );
            int obstcl_pos_y = rand() % ( SCREEN_HEIGHT - obstcl_height + 1 );
            if ( obstacles.size() > MAX_OBSTCLS )
            {
                obstacles.erase( obstacles.begin() );
            }
            obstacles.push_back( Obstacle(600, obstcl_pos_y, 20, obstcl_height ) );
            start_ticks = SDL_GetTicks();
        }

        // Move the obstacle
        for( std::vector< Obstacle >::iterator i = obstacles.begin();
             i < obstacles.end(); ++i )
        {
            (*i).move();
        }

        //obstcl.move()
        // Move our entity around
        if( !ent.move( SCREEN_WIDTH, SCREEN_HEIGHT, obstacles ) )
        {
            quit = 1;
        }

        // Clear screen
        SDL_SetRenderDrawColor( glob_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear( glob_renderer );

        // Render the obstacle rectangle and the entity circle
        SDL_SetRenderDrawColor( glob_renderer, 0x00, 0x00, 0x00, 0xFF);

        // Render all obstacles
        for( std::vector< Obstacle >::iterator i = obstacles.begin();
             i < obstacles.end(); ++i )
        {
            (*i).render( glob_renderer );
        }
        //obstcl.render( glob_renderer );
        ent.render( glob_renderer );

        // Update the renderer
        SDL_RenderPresent( glob_renderer );

    }
}
