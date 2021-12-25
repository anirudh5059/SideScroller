#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
#include"texture.hpp"

Texture::Texture( SDL_Renderer* lRenderer )
{
    raw_texture = NULL;
    width = 0;
    height = 0;
    renderer = lRenderer;
}

Texture::~Texture()
{
    free();
}

void Texture::free()
{
    if( raw_texture != NULL )
    {
        SDL_DestroyTexture( raw_texture );
        width = 0;
        height = 0;
    }
}

int Texture::get_width()
{
    return width;
}

int Texture::get_height()
{
    return height;
}

bool Texture::load_path( std::string path, int color_key )
{
    // First create an SDL_Surface, then create an SDL_Texture from that which
    // will be rendered in the GPU hardware
    SDL_Texture* local_texture = NULL;
    SDL_Surface* local_surface = IMG_Load( path.c_str() );
    if( local_surface == NULL )
    {
       printf( "Could not load surface %s!, SDL error %s\n", path.c_str(),
                SDL_GetError() ); 
    }
    else
    {
        // Color keying allows us to specify which color in the surface to
        // treat as transparent while rendering
        if( color_key == 1 )
        {
	        SDL_SetColorKey( local_surface, SDL_TRUE,
                        SDL_MapRGB( local_surface->format, 0x00, 0x00, 0x00 ) );
        }
        if( color_key == 2 )
        {
	        SDL_SetColorKey( local_surface, SDL_TRUE,
                        SDL_MapRGB( local_surface->format, 0xFF, 0xFF, 0xFF ) );
        }

        //Converting surface to texture
        local_texture = SDL_CreateTextureFromSurface( renderer, local_surface );
        if( local_texture == NULL )
        {
            printf( "Could not create texture from surface %s!, SDL Error %s\n",
                    path.c_str(), SDL_GetError() );
        }
        else
        {
            width = local_surface->w;
            height = local_surface->h; 
        }
        // Now that the requirement of SDL_Surface is complete, free it
        SDL_FreeSurface( local_surface );
    }
    raw_texture = local_texture;
    return raw_texture != NULL;
}

void Texture::render( int x, int y, SDL_Rect* clip, SDL_Rect* out )
{
    // Stack variable so we don't have to free it!
    SDL_Rect render_quad = { x, y, width, height };

    // If output dimensions are defined, use them
    if( out != NULL )
    {
        render_quad.w = out->w;
        render_quad.h = out->h;
    }
    // Clip is used as a mask to the part of the texture to be rendered
    if( clip != NULL)
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    // Perform the actual rendering
    SDL_RenderCopy( renderer, raw_texture, clip, &render_quad );
}

bool Texture::load_from_rendered_text( std::string& texture_text,
                                       SDL_Color& text_color, TTF_Font* font )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* text_surface = TTF_RenderText_Solid( font,
                                                      texture_text.c_str(),
                                                      text_color );
    if( text_surface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n",
                TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        raw_texture = SDL_CreateTextureFromSurface( renderer, text_surface );
        if( raw_texture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error:" 
                    "%s\n",
                    SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            width = text_surface->w;
            height = text_surface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( text_surface );
    }
    
    //Return success
    bool success = ( raw_texture != NULL );
    return success;
}
