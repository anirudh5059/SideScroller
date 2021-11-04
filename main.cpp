#include<SDL.h>
#include<SDL_image.h>
#include<string>
#include<vector>
#include"entity.hpp"
#include"obstacle.hpp"
#include"texture.hpp"

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

SDL_Window* glob_win;
SDL_Renderer* glob_renderer;
const int INTERVAL = 3000;
const int MAX_OBSTCLS = 5;
const int OBSTCL_WIDTH = 40;
const int OBSTCL_HEIGHT = 250;
const int OBSTCL_X = 600;
const int OBSTCL_VEL = -1;
const int ENT_X = 100;
const int ENT_Y = 100;
const int ENT_MIN_R = 20;
const int ENT_MAX_R = 120;

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

bool load( Texture& back, Texture& sprite, std::string path_back,
           std::string path_sprite )
{
    if( !back.load_path( path_back.c_str() ) )
    {
        printf( "Error loading background\n" );
    }
    if( !sprite.load_path( path_sprite.c_str(), true ) )
    {
        printf( "Error loading column sprite\n" );
    }
}

void inc_mod( int& idx, int base )
{
    idx = ( ( ++idx ) % base );
}

int main()
{
    if ( !init( true) )
    {

        printf( "Could not initialize, exiting\n" );
        return (-1);
    }

    Texture background( glob_renderer ); 
    Texture sprite( glob_renderer ); 
    
    load( background, sprite, "images/back2.png", "images/column_sprite.png" );

    MDEntity ent( ENT_X, ENT_Y, ENT_MIN_R, ENT_MAX_R );
    Obstacle obstacles[ MAX_OBSTCLS ];
    int OBS_IDX = 0;

    /*
    for( int i = 0; i < MAX_OBSTCLS; ++i)
    {
        obstacles[i] = new Obstacle();
    }
    */

    //obstacles.push_back( Obstacle ( 600, 10, 20, 250 ) );
    //Obstacle obstcl( 600, 10, 20, 250 );
    SDL_Event e;
    Uint32 start_ticks = 0;
    srand(SDL_GetTicks());
    bool quit = false;

    obstacles[ OBS_IDX ].resuscitate( OBSTCL_X, 10, OBSTCL_WIDTH, OBSTCL_HEIGHT, OBSTCL_VEL );
    obstacles[ OBS_IDX ].attach_sprite( &sprite );
    inc_mod( OBS_IDX, MAX_OBSTCLS );
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
            // Debug
            //printf( "The current obstacle Index %d\n", OBS_IDX );
            /*
            int obstcl_height = 100 + ( rand() % ( SCREEN_HEIGHT
                                                   -100 
                                                   - 5 * ent.get_min_rad()
                                                   + 1 ) );
            */
            int obstcl_pos_y = rand() % ( SCREEN_HEIGHT - OBSTCL_HEIGHT + 1 );
            obstacles[ OBS_IDX ].resuscitate( OBSTCL_X, obstcl_pos_y, OBSTCL_WIDTH, OBSTCL_HEIGHT, -1 );
            obstacles[ OBS_IDX ].attach_sprite( &sprite );
            inc_mod( OBS_IDX, MAX_OBSTCLS );
            /*
            if ( obstacles.size() > MAX_OBSTCLS )
            {
                obstacles.erase( obstacles.begin() );
            }
            obstacles.push_back( Obstacle(600, obstcl_pos_y, 20, obstcl_height ) );
            */
            start_ticks = SDL_GetTicks();
        }

        // Move the obstacle
        for( int i = 0; i < MAX_OBSTCLS; ++i )
        {
            obstacles[i].move();
        }

        //obstcl.move()
        // Move our entity around
        if( !ent.move( SCREEN_WIDTH, SCREEN_HEIGHT, obstacles, MAX_OBSTCLS ) )
        {
            quit = 1;
        }

        // Clear screen
        SDL_SetRenderDrawColor( glob_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear( glob_renderer );

        background.render( 0, 0, NULL, NULL ); 

        // Render the obstacle rectangle and the entity circle
        // Forest green
        //SDL_SetRenderDrawColor( glob_renderer, 0x1F, 0x7D, 0x1F, 0xFF);
        // Evening Blue
        //SDL_SetRenderDrawColor( glob_renderer, 0x01, 0x4F, 0x94, 0xFF);
        // Light blue
        SDL_SetRenderDrawColor( glob_renderer, 0xAD, 0xD8, 0xE6, 0xFF);
        //SDL_SetRenderDrawColor( glob_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        // Render all obstacles
        for( int i = 0; i < MAX_OBSTCLS; ++i )
        {
            obstacles[i].render( glob_renderer );
        }
        //obstcl.render( glob_renderer );
        ent.render( glob_renderer );

        // Update the renderer
        SDL_RenderPresent( glob_renderer );

    }
}
