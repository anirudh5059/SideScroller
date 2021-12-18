#include<SDL.h>
#include<SDL_image.h>
#include<string>
#include<vector>
#include "entity.hpp"
#include "obstacle.hpp"
#include "texture.hpp"
#include "consts.hpp"

const int SCREEN_WIDTH=640;
const int SCREEN_HEIGHT=480;

SDL_Window* glob_win;
SDL_Renderer* glob_renderer;

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

void generate_entities( int& obs_idx, int& pix_idx, Obstacle obstacles[], 
                        Pixie pixies[], Texture& obstcl_sprite, 
                        bool obstcl_ping )
{
    int pix_pos_y = 0;
    if( obstcl_ping )
    {
        int obstcl_pos_y = rand() % ( SCREEN_HEIGHT - OBSTCL_HEIGHT + 1 );
        obstacles[ obs_idx ].resuscitate( OBSTCL_X, obstcl_pos_y,
                                          OBSTCL_WIDTH, OBSTCL_HEIGHT,
                                          OBSTCL_VEL, IMMOBILE_VEL );
        obstacles[ obs_idx ].attach_sprite( &obstcl_sprite );
        inc_mod( obs_idx, MAX_OBSTCLS );
        if( obstcl_pos_y < ( ( SCREEN_HEIGHT - OBSTCL_HEIGHT ) / 2 ) )
        {
            pix_pos_y = obstcl_pos_y + OBSTCL_HEIGHT + PIXIE_RAD + ( rand()
                        % ( SCREEN_HEIGHT - obstcl_pos_y - OBSTCL_HEIGHT
                            - 2 * ( PIXIE_RAD ) ) );
        }
        else
        {
            pix_pos_y = PIXIE_RAD + rand() % ( obstcl_pos_y
                                               - 2 * ( PIXIE_RAD ) );
        }
    }
    else
    {
        pix_pos_y = PIXIE_RAD + ( rand() % ( SCREEN_HEIGHT
                                             - 2 * ( PIXIE_RAD ) ) );
    }
    pixies[ pix_idx ].resuscitate( OBSTCL_X, pix_pos_y, PIXIE_RAD,
                                   OBSTCL_VEL, IMMOBILE_VEL );
    inc_mod( pix_idx, MAX_PIXIES );
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

    MDEntity ent( ENT_MIN_R, ENT_MAX_R );
    ent.populate( ENT_X, ENT_Y, SCREEN_WIDTH, SCREEN_HEIGHT );
    Obstacle obstacles[ MAX_OBSTCLS ];
    Pixie pixies[ MAX_PIXIES ];
    int obs_idx = 0;
    int pix_idx = 0;

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
    int interval_count = 0;
    srand(SDL_GetTicks());
    bool quit = false;

    obstacles[ obs_idx ].resuscitate( OBSTCL_X, OBSTCL_Y, OBSTCL_WIDTH,
                                      OBSTCL_HEIGHT, OBSTCL_VEL, IMMOBILE_VEL );
    obstacles[ obs_idx ].attach_sprite( &sprite );
    inc_mod( obs_idx, MAX_OBSTCLS );
    long long score = 0;
    int move_res;
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
            interval_count++;
            bool obstcl_ping = false;
            if( interval_count > 4 )
            {
                obstcl_ping = true;
                interval_count = 0;
            }
            start_ticks = SDL_GetTicks();
            generate_entities( obs_idx, pix_idx, obstacles, pixies,
                               sprite, obstcl_ping );
            // Debug
            //printf( "The current obstacle Index %d\n", obs_idx );
            /*
            int obstcl_height = 100 + ( rand() % ( SCREEN_HEIGHT
                                                   -100 
                                                   - 5 * ent.get_min_rad()
                                                   + 1 ) );
            */
            /*
            if ( obstacles.size() > MAX_OBSTCLS )
            {
                obstacles.erase( obstacles.begin() );
            }
            obstacles.push_back( Obstacle(600, obstcl_pos_y, 20, obstcl_height ) );
            */
        }

        // Move the obstacle
        for( int i = 0; i < MAX_OBSTCLS; ++i )
        {
            obstacles[i].move( SCREEN_WIDTH, SCREEN_HEIGHT );
        }

        for( int i = 0; i < MAX_PIXIES; ++i )
        {
            pixies[i].move( SCREEN_WIDTH, SCREEN_HEIGHT );
        }

        //obstcl.move()
        // Move our entity around
        move_res = ent.move( SCREEN_WIDTH, SCREEN_HEIGHT, obstacles,
                             MAX_OBSTCLS, pixies, MAX_PIXIES );

        if( move_res == NEG_INF )
        {
            quit = 1;
        }
        else
        {
            score += move_res;
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

        for( int i = 0; i < MAX_PIXIES; ++i )
        {
            pixies[i].render( glob_renderer );
        }
        //obstcl.render( glob_renderer );
        ent.render( glob_renderer );

        // Update the renderer
        SDL_RenderPresent( glob_renderer );

    }
}
