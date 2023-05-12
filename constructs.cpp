#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL.h>
#include<stdexcept>
#include<iostream>
#include "consts.hpp"
#include "constructs.hpp"

void delete_window( SDL_Window *window )
{
    if( window )
    {
	    SDL_DestroyWindow( window );
    }
}

void delete_renderer( SDL_Renderer *renderer )
{
    if( renderer )
    {
        SDL_DestroyRenderer( renderer );
    }
}

// Remember, class objects are constructed in the order they are declared, so when
// we get to constructing the textures, the renderer is already populated.
GameConstructs::GameConstructs():
    window( nullptr, delete_window ), renderer( nullptr, delete_renderer ),
    background( renderer ), score( renderer ),
    ent( ENT_MIN_R, ENT_MAX_R, ENT_X, ENT_Y, SCREEN_WIDTH, SCREEN_HEIGHT ),
    obs_idx(0), pix_idx(0), screen_rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }
{
    for( int i=0; i<MAX_OBSTCLS; ++i )
    {
        obstacles.push_back( Obstacle( renderer ) );
    }
    for( int i=0; i<MAX_PIXIES; ++i )
    {
        pixies.push_back( Pixie( renderer ) );
    }
}

void GameConstructs::init( bool png_load, bool ttf_load, std::string path_back,
        std::string path_sprite, std::string path_pix, std::string path_font ) {
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Error initializing SDL: %s\n ", SDL_GetError() );
        throw std::runtime_error( "Could not initialize SDL" );
    }
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        printf( "Could not enable linear texture filtering, continuing\n" );
    }
    
    window = std::unique_ptr<SDL_Window, void(*)( SDL_Window* )>( SDL_CreateWindow( "PetProj",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN ),
            delete_window );
    if( !window )
    {
        printf( "Window could not be created, SDL Error: %s\n",
                SDL_GetError() );
        throw std::runtime_error( "Could not create SDL_Window" );
    }
    renderer = std::shared_ptr<SDL_Renderer>(
            SDL_CreateRenderer( window.get(), -1,
                ( SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) ),
            delete_renderer );
    if( !renderer )
    {
       printf( "Renderer could not be created, SDL Error: %s\n",
                SDL_GetError );
        throw std::runtime_error( "Could not create SDL_Renderer" );
    }
    background.renderer = renderer;
    for( auto& obs : obstacles )
    {
        obs.sprite.renderer = renderer;
    }
    for( auto& pix : pixies )
    {
        pix.sprite.renderer = renderer;
    }
    score.font_tex.renderer = renderer;
    SDL_SetRenderDrawColor( renderer.get(), 0xFF, 0xFF, 0xFF,
            0xFF );

    // Load the SDL_Image functionality if required
    if( png_load )
    {
        int img_flags = IMG_INIT_PNG;  
        if( !(IMG_Init( img_flags ) & img_flags ) )
        {
            printf( "SDL_Image could not be loaded, SDL Error: %s\n",
                    SDL_GetError() );
            throw std::runtime_error( "Could not load SDL_Image" );
        }
    }
    if( ttf_load )
    {
        //Initialize SDL_ttf
        if( TTF_Init() == -1 )
        {
            printf( "SDL_ttf could not initialize! SDL_ttf Error:"
                    "%s\n",
                    TTF_GetError() );
            throw std::runtime_error( "Could not load SDL_ttf" );
        }
    }
    background.load_path( path_back );
    for( auto& obs : obstacles ) {
        obs.sprite.load_path( path_sprite );
    }
    for( auto& pix : pixies ) {
        pix.sprite.load_path( path_pix, 2 );
    }

    //Open the font
    score.init_font( path_font );
    if( !score.font )
    {
        printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
        throw std::runtime_error( "Font could not be loaded" );
    }
}

void GameConstructs::generate_environment( bool obstcl_ping )
{
    int pix_pos_y = 0;
    if( obstcl_ping )
    {
        int obstcl_pos_y = rand() % ( SCREEN_HEIGHT - OBSTCL_HEIGHT + 1 );
        obstacles[ obs_idx ].resuscitate( OBSTCL_X, obstcl_pos_y,
                                          OBSTCL_WIDTH, OBSTCL_HEIGHT,
                                          OBSTCL_VEL, IMMOBILE_VEL );
            obstacles[ obs_idx ].rect.w;
        obs_idx = ( obs_idx + 1 ) % MAX_OBSTCLS;
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
    pix_idx = ( pix_idx + 1 ) % MAX_PIXIES;
}

void GameConstructs::move_environment()
{
    for( auto& obs : obstacles )
    {
        obs.move( SCREEN_WIDTH, SCREEN_HEIGHT );
    }

    for( auto& pix : pixies )
    {
        pix.move( SCREEN_WIDTH, SCREEN_HEIGHT );
    }
}

void GameConstructs::move_entity()
{
    int mov_res = ent.move( SCREEN_WIDTH, SCREEN_HEIGHT, obstacles, pixies );
    if( mov_res == NEG_INF )
    {
        throw std::runtime_error( "Collision occured, exiting" );
    }
    else
    {
        score.value += mov_res;
    }
}
void GameConstructs::render()
{

    //SDL_SetRenderDrawColor( constructs.renderer, 0x1F, 0x7D, 0x1F, 0xFF);
    // Evening Blue
    //SDL_SetRenderDrawColor( constructs.renderer, 0x01, 0x4F, 0x94, 0xFF);
    // Light blue
    //SDL_SetRenderDrawColor( constructs.renderer, 0xAD, 0xD8, 0xE6, 0xFF);
    // white
    //SDL_SetRenderDrawColor( constructs.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    // Pastel red
    // SDL_SetRenderDrawColor( constructs.renderer, 0xFF, 0x69, 0x61, 0xFF);
    // Red
    //SDL_SetRenderDrawColor( constructs.renderer, 0xFF, 0x00, 0x00, 0xFF);

    try
    {
        SDL_SetRenderDrawColor( renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear( renderer.get() );
        background.render( 0, 0, NULL, &screen_rect );
        // SDL_SetRenderDrawColor( renderer.get(), 0xFF, 0x69, 0x61, 0xFF);
        for( auto& obs : obstacles )
        {
            obs.render();
        }

        for( auto& pix : pixies )
        {
            pix.render();
        }
        // Light blue
        SDL_SetRenderDrawColor( renderer.get(), 0xAD, 0xD8, 0xE6, 0xFF);
        ent.render( renderer );
        score.render();
    }
    
    catch(...)
    {
        throw;
    }
    SDL_RenderPresent( renderer.get() );
}

GameConstructs::~GameConstructs()
{
    SDL_Quit();
}
