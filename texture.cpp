#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>
#include<memory>
#include<stdexcept>
#include<iostream>
#include"texture.hpp"
#include "consts.hpp"

void delete_texture( SDL_Texture* texture )
{
    if( texture )
    {
        SDL_DestroyTexture( texture );
    }
}

Texture::Texture( std::shared_ptr<SDL_Renderer> _renderer ):
    texture(nullptr, delete_texture), width(0), height(0), renderer(_renderer) {}

int Texture::get_width()
{
    return width;
}

int Texture::get_height()
{
    return height;
}

void ObjectTexture::load_path( std::string path, int color_key )
{
    // First create an SDL_Surface, then create an SDL_Texture from that which
    // will be rendered in the GPU hardware
    
    std::unique_ptr<SDL_Surface, void (*)( SDL_Surface* )> local_surface(
            IMG_Load( path.c_str() ),
            [] (SDL_Surface* surf) { SDL_FreeSurface( surf ); } );
    if( !local_surface )
    {
       printf( "Could not load surface %s!, SDL error %s\n", path.c_str(),
                SDL_GetError() ); 
       throw std::runtime_error( "Could not load surface passed" );
    }

    // Color keying allows us to specify which color in the surface to
    // treat as transparent while rendering
    if( color_key == 1 )
    {
        SDL_SetColorKey( local_surface.get(), SDL_TRUE,
                    SDL_MapRGB( local_surface->format, 0x00, 0x00, 0x00 ) );
    }
    if( color_key == 2 )
    {
        SDL_SetColorKey( local_surface.get(), SDL_TRUE,
                    SDL_MapRGB( local_surface->format, 0xFF, 0xFF, 0xFF ) );
    }

    //Converting surface to texture
    try
    {
        texture_from_surface( std::move( local_surface ) );
    }
    catch( ... )
    {
        throw;
    }
}

void Texture::texture_from_surface(
        std::unique_ptr<SDL_Surface, void (*)( SDL_Surface* )> surf )
{
    texture = std::unique_ptr< SDL_Texture, void (*)( SDL_Texture* texture ) > (
            SDL_CreateTextureFromSurface( renderer.get(), surf.get() ),
            delete_texture );
    if( !texture )
    {
        throw std::runtime_error( "Could not create texture from surface" );
    }
    width = surf->w;
    height = surf->h;
    
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
    SDL_RenderCopy( renderer.get(), texture.get(), clip, &render_quad );
}

void FontTexture::load_text( std::string texture_text,
                                       SDL_Color text_color, TTF_Font* font )
{

    //Render text surface
    std::unique_ptr<SDL_Surface, void (*)( SDL_Surface* )> text_surface(
            TTF_RenderText_Solid( font, texture_text.c_str(), text_color ),
            [] (SDL_Surface* surf) { SDL_FreeSurface( surf ); } );
    if( !text_surface )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n",
                TTF_GetError() );
        throw std::runtime_error( "Unable to render text surface in SDL_ttf" );
    }
    // Create texture from surface pixels
    try
    {
        texture_from_surface( std::move( text_surface ) );
    }
    catch( ... )
    {
        throw;
    }

}
ObjectTexture::ObjectTexture( std::shared_ptr<SDL_Renderer> _renderer ):
    Texture(_renderer) {}

FontTexture::FontTexture( std::shared_ptr<SDL_Renderer> _renderer ):
    Texture(_renderer) {}
